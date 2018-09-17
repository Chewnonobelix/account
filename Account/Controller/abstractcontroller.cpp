#include "abstractcontroller.h"

QString AbstractController::m_account = QString();
QMultiMap<QDate, Entry> AbstractController::m_entry = QMultiMap<QDate, Entry>();
ControllerDB AbstractController::m_db = ControllerDB();

AbstractController::AbstractController(): QObject(nullptr)
{

}

AbstractController::~AbstractController()
{}

QStringList AbstractController::accountList()
{
    return m_db.selectAccount();
}

void AbstractController::setCurrentAccount(QString a)
{
    m_account = a;
    m_entry.clear();
    auto l = m_db.selectEntry(a);
    for(auto it: l)
        m_entry.insert(it.date(), it);

    initTestEntry();
}

QString AbstractController::currentAccount()
{
    return m_account;
}


void AbstractController::addEntry(const Entry& e)
{
    if(m_db.addEntry(e))
    {
        auto l = m_db.selectEntry(currentAccount());
        m_entry.clear();

        for(auto it: l)
            m_entry.insert(it.date(), it);

    }
}

void AbstractController::removeEntry(const Entry& e)
{
    //m_entry.remove(e.date(), e);
}

Entry AbstractController::entry(int id)
{
    Entry ret;

    for(auto it: m_entry)
        if(it.id() == id)
            ret = it;

    return ret;
}

QList<Entry> AbstractController::entries(QDate d)
{
    if(d == QDate())
        return m_entry.values();
    else
        return m_entry.values(d);
}

void AbstractController::initTestEntry()
{
    for(int i = 0; i < 5; i++)
    {
        Entry e;
        QRandomGenerator::global()->bounded(999.8);
        e.setId(QRandomGenerator::global()->generate() % 264);
        e.setAccount(currentAccount());
        e.setValue(QRandomGenerator::global()->generateDouble() *100);
        e.setDate(QDate::currentDate());

        if(i%5 == 0)
            e.setType("income");
        else
            e.setType("outcome");

        Information in;
        in.setTitle(QString::number(e.id()));
        e.setInfo(in);
//        qDebug()<<"Gen"<<e.id()<<e.value();

        m_entry.insert(e.date(), e);
    }


//    for(auto e: m_entry)
//        qDebug()<<e.id()<<e.value();
}
