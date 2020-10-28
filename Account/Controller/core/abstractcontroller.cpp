#include "abstractcontroller.h"

QString AbstractController::m_account = QString();
InterfaceDataSave* AbstractController::m_db = nullptr;
QMap<QDate, Total> AbstractController::m_accountTotal = QMap<QDate, Total>();
QThread* AbstractController::m_dbThread = nullptr;
QSharedPointer<QMutex> AbstractController::CalcThread::mut = QSharedPointer<QMutex>::create();
QList<QSharedPointer<AbstractController::CalcThread>> AbstractController::pool = QList<QSharedPointer<AbstractController::CalcThread>>();
QSet<int> AbstractController::CalcThread::indexes = QSet<int>();
int AbstractController::CalcThread::nbRunning = 0;

AbstractController::AbstractController(): QObject(nullptr)
{
    
}


AbstractController::~AbstractController()
{
    
}

AbstractController::CalcThread::CalcThread(int index, QList<Entry> l, QMap<QDate, Total>* r):index(index), l(l), ret(r)
{}

void AbstractController::setCurrentAccount(QString a)
{
    db()->setCurrentAccount(a);
}

QString AbstractController::currentAccount()
{
    return db()->currentAccount();
}

void AbstractController::CalcThread::run()
{
    int start = std::max(l.size() / 4, 1);
    start *= index;
    int j = 0;
    
    for(j = start; j < (start + (std::max(l.size() / 4, 1))) && j < l.size(); j++)
    {
        if(indexes.contains(j) || !ret)
            continue;
        
        mut->lock();
        Total t = (*ret)[l[j].date()];
        indexes<<j;
        t = t + (l)[j];
        
        (*ret)[l[j].date()] = t;
        mut->unlock();
    }
}

void AbstractController::finishTotalThread()
{
    CalcThread::nbRunning --;
    
    if(m_accountTotal.size() > 1)
    {
        for(auto it = m_accountTotal.begin()+1; it != m_accountTotal.end() && CalcThread::nbRunning == 0; it++)
        {
            *it = *it + *(it-1);
        }
    }

    emit s_totalChanged(QDate::currentDate().month()+1, QDate::currentDate().year());
}

void AbstractController::calculTotal()
{
    if(CalcThread::nbRunning != 0)
        return;
    
    pool.clear();
    CalcThread::indexes.clear();
    m_accountTotal.clear();
    auto l = m_db->selectEntry().values();
    std::sort(l.begin(), l.end(), [](const Entry& e1, const Entry& e2) {
            return e1.date() < e2.date();
    });

    for (auto i = 0; i < 5; i++) {
        auto t = QSharedPointer<CalcThread>::create(i, l, &m_accountTotal);
        pool<<t;
        t->start();
        CalcThread::nbRunning ++;
        connect(t.data(), &CalcThread::finished, this, &AbstractController::finishTotalThread);
    }
}

void AbstractController::addEntry(Entry &e)
{
    Entry et = e;

    et.setEstimated(et.date() > QDate::currentDate());

    if (m_db->addEntry(et)) {
        Entry init;
        for (auto it : m_db->selectEntry())
            if (it.title() == "Initial")
                init = it;
        
        if(et.date() < init.date())
        {
            QDate nd = et.date().addDays(-1);
            
            
            init.setDate(nd);
            double val = init.value();
            val -= (et.value() * int(et.type()));
            init.setValue(val);
            updateEntry(init);
        }
    }
}

Entry AbstractController::entry(QUuid id)
{    
    return m_db->selectEntry()[id];
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
    connect(m_dbThread, &QThread::started, m_db, &InterfaceDataSave::exec);
}

void AbstractController::deleteDb()
{
    if(m_db != nullptr)
        delete m_db;
}

void AbstractController::updateEntry(Entry & e)
{
    m_db->updateEntry(e);
}

Total AbstractController::accountTotal()
{
    return m_accountTotal.isEmpty() ? Total(): m_accountTotal.last();
}

QMap<QDate, Total> AbstractController::allTotal()
{
    return m_accountTotal;
}

InterfaceDataSave *AbstractController::db()
{
    return m_db;
}
