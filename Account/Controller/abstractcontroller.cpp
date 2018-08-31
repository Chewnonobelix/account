#include "abstractcontroller.h"

QStringList AbstractController::m_listAccount = QStringList();
QString AbstractController::m_account = QString();
QMultiMap<QDate, Entry> AbstractController::m_entry = QMultiMap<QDate, Entry>();

AbstractController::AbstractController(): QObject(nullptr)
{

}

AbstractController::~AbstractController()
{}

void AbstractController::setAccountList(QStringList l)
{
    m_listAccount = l;
}

QStringList AbstractController::accountList()
{
    return m_listAccount;
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
    m_entry.insert(e.date(), e);
}

void AbstractController::removeEntry(const Entry& e)
{
    m_entry.remove(e.date(), e);
}

