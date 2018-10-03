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
        m_updateEntry = new QSqlQuery(m_db);

        m_selectEntry->prepare("SELECT * FROM account AS a"
                               "INNER JOIN information AS i ON a.ID = i.id_entry "
                               "WHERE a.account=:a");

        m_addEntry->prepare("INSERT INTO account (account, value, date_eff, type) "
                            "VALUES (:account,:value,:date,:type)");

        m_accounts->prepare("SELECT DISTINCT account FROM account");

        m_updateEntry->prepare("UPDATE information "
                               "SET (info=:title, prev=:estimated)"
                               "WHERE id=:id AND id_entry=:ide");
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
    bool ret = false;
    if(isConnected() && e.id() < 0)
    {
        m_addEntry->bindValue(":account", QVariant(e.account()));
        m_addEntry->bindValue(":value", QVariant(e.value()));
        m_addEntry->bindValue(":date", QVariant(e.date()));
        m_addEntry->bindValue(":type", QVariant(e.type()));
    }

    return ret;
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
            Information i;
            t.setId(m_selectEntry->value("a.id").toInt());
            t.setDate(m_selectEntry->value("a.date_eff").toDate());
            t.setValue(m_selectEntry->value("a.value").toDouble());
            t.setType(m_selectEntry->value("a.type").toString());

            i.setId(m_selectEntry->value("i.id").toInt());
            i.setIdEntry(m_selectEntry->value("i.id_entry").toInt());
            i.setEstimated(m_selectEntry->value("i.prev").toBool());
            i.setTitle(m_selectEntry->value("i.info").toString());

            t.setInfo(i);
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

bool ControllerDB::updateEntry(const Entry & e)
{
    bool ret = false;
    if(isConnected() && e.id() >= 0)
    {
        m_updateEntry->bindValue(":ide", e.id());
        m_updateEntry->bindValue(":id", e.info().id());
        m_updateEntry->bindValue(":title", e.info().title());
        m_updateEntry->bindValue(":estimated", e.info().estimated());

        ret = m_updateEntry->exec();
    }

    return ret;
}
