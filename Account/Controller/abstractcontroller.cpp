#include "abstractcontroller.h"

QString AbstractController::m_account = QString();
InterfaceDataSave* AbstractController::m_db = nullptr;
Total AbstractController::m_accountTotal = Total();
QThread* AbstractController::m_dbThread = nullptr;

AbstractController::AbstractController(): QObject(nullptr)
{

}


AbstractController::~AbstractController()
{

}

void AbstractController::setCurrentAccount(QString a)
{
    m_account = a;
  
    calculTotal();
}

QString AbstractController::currentAccount()
{
    return m_account;
}

void AbstractController::calculTotal()
{
//    m_accountTotal = Total();
    
//    auto l = m_db->selectEntry(currentAccount());
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
    
    calculTotal();
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
    m_db = (InterfaceDataSave*)(QMetaType::create(type));
    m_db->init();
    m_db->moveToThread(m_dbThread);
    connect(m_dbThread, QThread::started, m_db, InterfaceDataSave::exec);
    m_dbThread->start();
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

    calculTotal();
}

Total AbstractController::accountTotal()
{
    return m_accountTotal;
}
