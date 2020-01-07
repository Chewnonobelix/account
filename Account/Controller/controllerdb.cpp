#include "controllerdb.h"
#include "dbrequestsinit.h"
#include <QDebug>
ControllerDB::ControllerDB()
{
}

ControllerDB::ControllerDB(const ControllerDB& d): InterfaceDataSave(d)
{}


ControllerDB::~ControllerDB()
{
    if(isConnected())
    {
        m_db.close();
    }
}

bool ControllerDB::init()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");

    m_db.setDatabaseName("account");
    m_db.open();

    if(m_db.tables().isEmpty())
    {
        auto req = m_db.exec(account_table);
    }

    if(isConnected())
    {
        prepareProfile();
        prepareEntry();
        prepareInformation();
        prepareAccount();
        prepareCategory();
        prepareBudget();
    }
    
    qDebug()<<"DB Connected"<<isConnected()<<m_db.tables();

    return isConnected();
}

void ControllerDB::prepareEntry()
{
    m_selectEntry = SqlQuery::create(m_db);
    m_addEntry = SqlQuery::create(m_db);
    m_removeEntry = SqlQuery::create(m_db);
    m_updateEntry = SqlQuery::create(m_db);
    m_selectMetadata = SqlQuery::create(m_db);
    m_insertMetadata = SqlQuery::create(m_db);
    m_removeMetadata = SqlQuery::create(m_db);
    m_updateMetadata = SqlQuery::create(m_db);

    m_selectEntry->prepare("SELECT * FROM account AS a"
                           "WHERE a.account=:a AND a.profile=:p");

    m_addEntry->prepare("INSERT INTO account (ID, account, value, date_eff, type, profile) "
                        "VALUES (:id, :account,:value,:date,:type, :profile)");

    m_removeEntry->prepare("DELETE FROM account"
                           "WHERE ID=:id");

    m_updateEntry->prepare("UPDATE account"
                           "SET (account=:a, value=:v, date_eff=:d, type=:t)"
                           "WHERE ID=:id");
    
    m_insertMetadata->prepare("INSERT INTO entrymetadata (id, entry, name, value)"
                              "VALUES (:id, :entry, :name, :value");
    
    m_updateMetadata->prepare("UPDATE entrymetada"
                              "SET (value=:value)"
                              "WHERE entry=:entry AND name=:name");
    
    m_selectMetadata->prepare("SELECT * FROM entrymetadata"
                              "WHERE entry=:ide");
    
    m_removeMetadata->prepare("DELETE FROM entrymetadata"
                              "WHERE entry=:ide AND name=:name");
}

void ControllerDB::prepareInformation()
{
    m_updateInfo = SqlQuery::create(m_db);
    m_addInformation = SqlQuery::create(m_db);
    m_removeInformation = SqlQuery::create(m_db);
    m_selectInformation = SqlQuery::create(m_db);

    m_addInformation->prepare("INSERT INTO information (id, id_entry, info, prev, category)"
                              "VALUES(:id, :ide, :title, :prev, :cat)");

    m_updateInfo->prepare("UPDATE information "
                           "SET (info=:title, prev=:estimated)"
                           "WHERE id=:id AND id_entry=:ide");

    m_removeInformation->prepare("DELETE FROM information"
                                 "WHERE id_entry=:ide");

    m_selectInformation->prepare("SELECT * FROM information"
                                 "WHERE id_entry = :ide");
}

void ControllerDB::prepareAccount()
{
    m_accounts = SqlQuery::create(m_db);
    m_removeAccount = SqlQuery::create(m_db);
 
    m_accounts->prepare("SELECT DISTINCT account FROM account WHERE profile=:profile");

    m_removeAccount->prepare("DELETE FROM account, categories, budget, frequency, commonExpanse"
                             "WHERE account=:a AND profile=:profile");
}

void ControllerDB::prepareCategory()
{
    m_addCategory = SqlQuery::create(m_db);
    m_removeCategory = SqlQuery::create(m_db);
    m_selectCategory = SqlQuery::create(m_db);

    m_addCategory->prepare("INSERT INTO categories(name, type, account, profile)"
                           "VALUES(:name, :type, :account, :profile)");

    m_removeCategory->prepare("DELETE FROM categories"
                              "WHERE name = :name AND account=:account AND profile=:profile");

    m_selectCategory->prepare("SELECT * FROM categories WHERE account=:account AND profile=:profile");
}

void ControllerDB::prepareBudget()
{
    m_addBudget = SqlQuery::create(m_db);
    m_removeBudget = SqlQuery::create(m_db);
    m_selectBudget = SqlQuery::create(m_db);
    m_updateBudget = SqlQuery::create(m_db);
    m_selectSubBudget = SqlQuery::create(m_db);
    m_removeSubbudget = SqlQuery::create(m_db);
    m_updateSubbudget = SqlQuery::create(m_db);
    m_addSubbudget = SqlQuery::create(m_db);

    m_addBudget->prepare("INSERT INTO budget (id, account, category, reference, profile)"
                         "VALUES (:id, :account, :category, :reference, :profile)");

    m_updateBudget->prepare("UPDATE budget"
                            "SET (category=:c, reference=:r)"
                            "WHERE (id=:id)");

    m_removeBudget->prepare("DELETE FROM budget"
                            "WHERE id=:id");

    m_selectBudget->prepare("SELECT * FROM budget"
                            "WHERE account=:a AND profile=:profile");
    
    m_selectSubBudget->prepare("SELECT * FROM subbudget"
                               "WHERE idBudget=:idb");
    
    m_updateSubbudget->prepare("UPDATE subbudget"
                               "SET (frequency=:freq, target=:target)"
                               "WHERE idBudget=:idb AND fromDate=:date");
    
    m_removeSubbudget->prepare("DELETE FROM subbudget"
                               "WHERE idBudget:idb AND fromDate=:date");
    
    m_addSubbudget->prepare("INSERT INTO subbudget (ID, idBudget, frequency, target, fromDate)"
                            "VALUES (:id, :idb, :frequency, :target, :date");
}

void ControllerDB::prepareFrequency()
{
    m_addFrequency = SqlQuery::create(m_db);
    m_removeFrequency = SqlQuery::create(m_db);
    m_selectFrequency = SqlQuery::create(m_db);
    m_updateFrequency = SqlQuery::create(m_db);
    m_addFrequencyReference = SqlQuery::create(m_db);
    m_removeFrequencyReference = SqlQuery::create(m_db);
    m_selectFrequencyReference = SqlQuery::create(m_db);
    m_updateFrequencyReference = SqlQuery::create(m_db);
    
    m_selectFrequency->prepare("SELECT * FROM frequency"
                               "WHERE account=:a AND profile=:profile");

    m_addFrequency->prepare("INSERT INTO frequency (ID, freq, nbGroup"
                            "VALUES(:id, :freq, :nbGroup)");

    m_removeFrequency->prepare("DELETE FROM frequency"
                               "WHERE (ID=:id)" );

    m_updateBudget->prepare("UPDATE frequency"
                            "SET (freq=:f, nbGroup=:ng)"
                            "WHERE (ID=:id");
}

void ControllerDB::prepareCommon()
{
    m_addCommon = SqlQuery::create(m_db);
    m_selectCommon = SqlQuery::create(m_db);
    m_removeCommon = SqlQuery::create(m_db);
    m_updateCommon = SqlQuery::create(m_db);
    //TODO
}

void ControllerDB::prepareProfile()
{
    m_selectProfiles = SqlQuery::create(m_db);
    m_removeProfile = SqlQuery::create(m_db);
    
    m_selectProfiles->prepare("SELECT DISTINCT profile FROM account");
    
    m_removeProfile->prepare("DELETE FROM account, categories, budget, frequency, commonExpanse"
                             "WHERE profile=:profile");
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
        m_addEntry->bindValue(":id", QVariant());

        ret = m_addEntry->exec();
        int id = m_addEntry->lastInsertId().toInt();

        if(ret && id > 0)
        {
            m_addInformation->bindValue(":ide", id);
            m_addInformation->bindValue(":title", e.info().title());
            m_addInformation->bindValue(":prev",e.info().estimated());
            m_addInformation->bindValue(":cat",e.info().category());
            qDebug()<<ret<<id;

            ret &= m_addInformation->exec();
            qDebug()<<ret;
        }
    }

    return ret;
}

QMultiMap<QDate, Entry> ControllerDB::selectEntry(QString account)
{
    QMultiMap<QDate, Entry> res;

    if(!isConnected())
        return res;

    m_selectEntry->bindValue(":a", QVariant(account));
    if(m_selectEntry->exec())
    {
        qDebug()<<"Find?";
        while(m_selectEntry->next())
        {
            Entry t;
            Information i;
            t.setId(m_selectEntry->value("a.id").toInt());
            t.setDate(m_selectEntry->value("a.date_eff").toDate());
            t.setValue(m_selectEntry->value("a.value").toDouble());
            t.setType(m_selectEntry->value("a.type").toString());

            m_selectInformation->bindValue(":ide", t.id());
            bool inf = m_selectInformation->exec();
            if(!inf)
            {
                qDebug()<<m_selectInformation->lastError().text();
                break;
            }

            i.setId(m_selectInformation->value("id").toInt());
            i.setIdEntry(m_selectInformation->value("id_entry").toInt());
            i.setEstimated(m_selectInformation->value("prev").toBool());
            i.setTitle(m_selectInformation->value("info").toString());

            t.setInfo(i);
            res.insert(t.date(), t);
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

bool ControllerDB::updateInfo(const Entry & e)
{
    bool ret = false;
    if(isConnected() && e.id() >= 0)
    {
        m_updateInfo->bindValue(":ide", e.id());
        m_updateInfo->bindValue(":id", e.info().id());
        m_updateInfo->bindValue(":title", e.info().title());
        m_updateInfo->bindValue(":estimated", e.info().estimated());

        ret = m_updateInfo->exec();
    }

    return ret;
}


bool ControllerDB::removeEntry(const Entry & e)
{
    bool ret = false;

    if(isConnected())
    {
        m_removeInformation->bindValue(":ide", e.id());
        m_removeEntry->bindValue(":id", e.id());

        ret = m_removeInformation->exec();

        if(ret)
            ret &= m_removeEntry->exec();
    }

    return ret;
}

bool ControllerDB::removeAccount(QString name)
{
    bool ret = false;

    if(isConnected())
    {
        m_removeAccount->bindValue(":a", name);
        ret = m_removeAccount->exec();
    }

    return ret;
}

bool ControllerDB::addCategory(QString n, QString t)
{
    if(isConnected())
    {
        m_addCategory->bindValue(":name", n);
        m_addCategory->bindValue(":type", t);

        return m_addCategory->exec();
    }

    return false;
}

bool ControllerDB::removeCategory(QString name)
{
    if(isConnected())
    {
        m_removeCategory->bindValue(":name", name);
        return m_removeCategory->exec();
    }

    return false;
}

QMultiMap<QString, QString> ControllerDB::selectCategory()
{
    QMultiMap<QString, QString> ret;
    if(isConnected() && m_selectCategory->exec())
    {
        while(m_selectCategory->next())
        {
            QString name = m_selectCategory->value("name").toString();
            QString type = m_selectCategory->value("type").toString();
            ret.insert(type, name);
        }
    }

    return ret;
}

bool ControllerDB::addBudget(const Budget&)
{
    //TODO
    return false;
}

bool ControllerDB::removeBudget(const Budget &)
{
    //TODO
    return false;
}

QList<Budget> ControllerDB::selectBudgets()
{
    //TODO
    return QList<Budget>();
}

bool ControllerDB::updateBudget(const Budget &)
{
    //TODO
    return false;
}

bool ControllerDB::updateEntry(const Entry & e)
{
//    m_updateEntry->prepare("UPDATE account"
//                           "SET (account=:a, value=:v, date_eff=:d, type=:t)"
//                           "WHERE ID=:id");

    bool ret = false;

    if(isConnected())
    {
        m_updateEntry->bindValue(":a", e.account());
        m_updateEntry->bindValue(":v", e.value());
        m_updateEntry->bindValue(":d", e.date());
        m_updateEntry->bindValue(":t", e.type());
        m_updateEntry->bindValue(":id", e.id());

        ret = m_updateEntry->exec();

        ret &= updateInfo(e);
    }


    return ret;
}

bool ControllerDB::addFrequency(const Frequency &)
{
    return false; //TODO
}

bool ControllerDB::removeFrequency(const Frequency&)
{
    return false; //TODO
}

bool ControllerDB::updateFrequency(const Frequency&)
{
    return false; //TODO
}

QList<Frequency> ControllerDB::selectFrequency()
{
    return QList<Frequency>(); //TODO
}
