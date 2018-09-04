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
