#include "abstractcontroller.h"

QString AbstractController::m_account = QString();
InterfaceDataSave* AbstractController::m_db = nullptr;
Total AbstractController::m_accountTotal = Total();
QThread* AbstractController::m_dbThread = nullptr;
QSharedPointer<QMutex> AbstractController::CalcThread::mut = QSharedPointer<QMutex>::create();
QList<QSharedPointer<AbstractController::CalcThread>> AbstractController::pool = QList<QSharedPointer<AbstractController::CalcThread>>();
QSet<int> AbstractController::CalcThread::indexes = QSet<int>();

AbstractController::AbstractController(): QObject(nullptr)
{

}


AbstractController::~AbstractController()
{

}

AbstractController::CalcThread::CalcThread(int index, QList<Entry> l, Total* r):index(index), l(l), ret(r)
{}

void AbstractController::setCurrentAccount(QString a)
{
    m_account = a;
  }

QString AbstractController::currentAccount()
{
    return m_account;
}

void AbstractController::CalcThread::run()
{
    qDebug()<<"Run"<<index;
    int start = l.size() / 4;
    start *= index;
    int j = 0;


    for(j = start; j < (start + (l.size() / 4)) && j < l.size(); j++)
    {
        qDebug()<<j;
        if(indexes.contains(j))
            continue;

        mut->lock();
        indexes<<j;
        *ret = *ret + (l)[j];
        mut->unlock();
    }
    qDebug()<<"Finish"<<index<<l.size();
}

void AbstractController::calculTotal()
{
    for(auto it: pool)
    {
        it->terminate();
        it->wait();
    }
    pool.clear();
    CalcThread::indexes.clear();
    m_accountTotal = Total();
    auto l = m_db->selectEntry(currentAccount()).values();
    for(auto i = 0; i < 5; i++)
    {
        pool<<QSharedPointer<CalcThread>::create(i, l, &m_accountTotal);
        pool.last()->start();
        connect(pool.last().data(), CalcThread::finished, this, AbstractController::s_totalChanged);
    }
    qDebug()<<"Pool"<<pool.size();
//    for(auto it: l)
//        m_accountTotal = m_accountTotal + it;    
}

void AbstractController::addEntry(const Entry& e)
{
    Entry et = e;
    Information i = et.info();
    i.setEstimated(et.date() > QDate::currentDate());
    et.setInfo(i);
    if(m_db->addEntry(et))
    {
        Entry init;
        for(auto it: m_db->selectEntry(currentAccount()))
            if(it.label() == "Initial")
                init = it;

        if(et.date() < init.date())
        {
            QMetaEnum qme = QMetaEnum::fromType<Account::TypeEnum>();
            QDate nd = et.date().addDays(-1);


            init.setDate(nd);
            double val = init.value();
            val -= (et.value()*qme.keysToValue(et.type().toLower().toLatin1()));
            init.setValue(val);
            updateEntry(init);
        }
    }
 }

Entry AbstractController::entry(int id)
{
    Entry ret;

    for(auto it: m_db->selectEntry(currentAccount()))
        if(it.id() == id)
            ret = it;

    return ret;
}

void AbstractController::setDb(QString name)
{
    int type = QMetaType::type(name.toLatin1());
    if(type == QMetaType::UnknownType)
        throw QString("Unknow DB type");

    if(m_db != nullptr)
    {
        m_db->thread()->terminate();
        m_db->thread()->wait();

        delete m_db;
    }
    m_dbThread->start();

    m_db = (InterfaceDataSave*)(QMetaType::create(type));
    m_db->init();
    m_db->moveToThread(m_dbThread);
    connect(m_dbThread, QThread::started, m_db, InterfaceDataSave::exec);
}

void AbstractController::deleteDb()
{
    if(m_db != nullptr)
        delete m_db;
}

void AbstractController::updateEntry(const Entry & e)
{
    m_db->updateEntry(e);
    m_db->updateInfo(e);
}

Total AbstractController::accountTotal()
{
    return m_accountTotal;
}
