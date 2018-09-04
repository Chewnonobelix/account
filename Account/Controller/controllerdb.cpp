#include "controllerdb.h"
#include <QDebug>
ControllerDB::ControllerDB()
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName("chewnonobelix.myqnapcloud.com");
    m_db.setUserName("chewnonobelix");
    m_db.setPassword("04091986a");
    m_db.setDatabaseName("account_test");
    if(m_db.open())
    {
        m_selectEntry = new QSqlQuery(m_db);
        m_addEntry = new QSqlQuery(m_db);
        m_accounts = new QSqlQuery(m_db);

        m_selectEntry->prepare("SELECT * FROM account WHERE account=:a");
        m_addEntry->prepare("INSERT INTO account (account, value, date_eff, type) VALUES (:account,:value,:date,:type)");
        m_accounts->prepare("SELECT DISTINCT account FROM account");
    }
}

ControllerDB::~ControllerDB()
{
    if(isConnected())
    {
        delete m_selectEntry;
        delete m_addEntry;
        delete m_accounts;

        m_db.close();
    }
}

bool ControllerDB::isConnected() const
{
    return m_db.isOpen();
}

bool ControllerDB::addEntry(const Entry & e)
{
    if(isConnected() && e.id() < 0)
    {
        m_addEntry->bindValue(":account", QVariant(e.account()));
        m_addEntry->bindValue(":value", QVariant(e.value()));
        m_addEntry->bindValue(":date", QVariant(e.date()));
        m_addEntry->bindValue(":type", QVariant(e.type()));

        return m_addEntry->exec();
    }

    return false;
}

QList<Entry> ControllerDB::selectEntry(QString account)
{
    QList<Entry> res;

    if(!isConnected())
        return res;

    m_selectEntry->bindValue(":a", QVariant(account));
    if(m_selectEntry->exec())
    {
        while(m_selectEntry->next())
        {
            Entry t;
            t.setId(m_selectEntry->value("id").toInt());
            t.setDate(m_selectEntry->value("date_eff").toDate());
            t.setValue(m_selectEntry->value("value").toDouble());
            t.setType(m_selectEntry->value("type").toString());

            res<<t;
        }
    }

    return res;
}

QStringList ControllerDB::selectAccount()
{
    QStringList res;
    if(isConnected() && m_accounts->exec())
    {
        qDebug()<<"Exec "<<m_accounts->lastQuery()<<m_accounts->executedQuery();

        while(m_accounts->next())
            res<<m_accounts->value("account").toString();
    }

    return res;
}

