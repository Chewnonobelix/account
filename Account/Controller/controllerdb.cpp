#include "controllerdb.h"

ControllerDB::ControllerDB()
{
    m_selectEntry.prepare("SELECT * FROM account WHERE account=:a");
    m_addEntry.prepare("INSERT INTO account (account, value, date_eff, type) VALUES (:account,:value,:date,:type)");
    m_accounts.prepare("SELECT DISTINCT account FROM account");
}

bool ControllerDB::addEntry(const Entry & e)
{
    if(e.id() < 0)
    {
        m_addEntry.bindValue(":account", QVariant(e.account()));
        m_addEntry.bindValue(":value", QVariant(e.value()));
        m_addEntry.bindValue(":date", QVariant(e.date()));
        m_addEntry.bindValue(":type", QVariant(e.type()));

        return m_addEntry.exec();
    }

    return false;
}

QList<Entry> ControllerDB::selectEntry(QString account)
{
    QList<Entry> res;

    m_selectEntry.bindValue(":a", QVariant(account));

    if(m_selectEntry.exec())
    {
        do
        {
            auto result = m_selectEntry.record();

            Entry t;
            t.setId(result.value("id").toInt());
            t.setDate(result.value("date_eff").toDate());
            t.setValue(result.value("value").toDouble());
            t.setType(result.value("type").toString());

            res<<t;

        }
        while(m_accounts.nextResult());
    }

    return res;
}

QStringList ControllerDB::selectAccount()
{
    QStringList res;

    if(m_accounts.exec())
    {
        do
        {
            auto result = m_accounts.record();

            res<<result.value(0).toString();

        }
        while(m_accounts.nextResult());
    }

    return res;
}

