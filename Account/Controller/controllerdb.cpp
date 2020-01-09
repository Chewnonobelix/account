#include "controllerdb.h"
#include "dbrequestsinit.h"
#include <QDebug>
ControllerDB::ControllerDB(): m_currentProfile("Default")
{
}

ControllerDB::ControllerDB(const ControllerDB& d): InterfaceDataSave(d), m_currentProfile(d.m_currentProfile)
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
    
    //    if(m_db.tables().isEmpty())
    {
        qDebug()<<"CT"<<m_db.exec(categories_table).lastError();
        qDebug()<<"FT"<<m_db.exec(frequency_table).lastError();
        qDebug()<<"BT"<<m_db.exec(budget_table).lastError();
        qDebug()<<"SBT"<<m_db.exec(subbudget_table).lastError();
        qDebug()<<"CEXT"<<m_db.exec(commonExpanse_table).lastError();
        qDebug()<<"AT"<<m_db.exec(account_table).lastError();
        qDebug()<<"EMT"<<m_db.exec(entrymetadata_table).lastError();
        qDebug()<<"CENT"<<m_db.exec(commonEntry_table).lastError();
        qDebug()<<"IT"<<m_db.exec(information_table).lastError();

        qDebug()<<"Trigger delete entry"<<m_db.exec(account_trigger_delete).lastError();
        qDebug()<<"Trigger delete frequency"<<m_db.exec(trigger_delete_frequency).lastError();
        qDebug()<<"Trigger delete category"<<m_db.exec(trigger_delete_category).lastError();
        qDebug()<<"Trigger delete common expanse"<<m_db.exec(trigger_delete_commonExpanse).lastError();
        qDebug()<<"Trigger delete common entry"<<m_db.exec(trigger_delete_commonEntry).lastError();
    }
    
    if(isConnected())
    {
        prepareProfile();
        prepareEntry();
        prepareInformation();
        prepareAccount();
        prepareCategory();
        prepareBudget();
        prepareFrequency();
        prepareCommon();
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
    
    qDebug()<<"SE"<<m_selectEntry->prepare("SELECT * FROM account "
                                           "WHERE account=:a AND profile=:p AND commonExpanse=0 AND frequencyReference IS NULL")<<m_selectEntry->lastError();
    
    qDebug()<<"AE"<<m_addEntry->prepare("INSERT INTO account (account, value, date_eff, type, profile) "
                                        "VALUES (:account,:value,:date,:type, :profile)")<<m_addEntry->lastError();
    
    qDebug()<<"RE"<<m_removeEntry->prepare("DELETE FROM account "
                                           "WHERE id=:id")<<m_removeEntry->lastError();
    
    qDebug()<<"UE"<<m_updateEntry->prepare("UPDATE account "
                                           "SET value=:v "
                                           "WHERE id=:id")<<m_updateEntry->lastError();
    
    qDebug()<<"AM"<<m_insertMetadata->prepare("INSERT INTO entrymetadata (entry, name, value) "
                                              "VALUES (:entry, :name, :value)")<<m_insertMetadata->lastError();
    
    qDebug()<<"UM"<<m_updateMetadata->prepare("UPDATE entrymetadata "
                                              "SET value=:value "
                                              "WHERE entry=:entry AND name=:name")<<m_updateMetadata->lastError();
    
    qDebug()<<"SM"<<m_selectMetadata->prepare("SELECT * FROM entrymetadata "
                                              "WHERE entry=:ide")<<m_selectMetadata->lastError();
    
    qDebug()<<"RM"<<m_removeMetadata->prepare("DELETE FROM entrymetadata "
                                              "WHERE entry=:ide AND name=:name")<<m_removeMetadata->lastError();
}

void ControllerDB::prepareInformation()
{
    m_updateInfo = SqlQuery::create(m_db);
    m_addInformation = SqlQuery::create(m_db);
    m_removeInformation = SqlQuery::create(m_db);
    m_selectInformation = SqlQuery::create(m_db);
    
    qDebug()<<"AI"<<m_addInformation->prepare("INSERT INTO information (idEntry, info, prev, category) "
                                              "VALUES(:ide, :title, :prev, :cat)")<<m_addInformation->lastError();
    
    qDebug()<<"UI"<<m_updateInfo->prepare("UPDATE information "
                                          "SET info=:title , prev=:estimated , category=:cat "
                                          "WHERE id = :id")<<m_updateInfo->lastError();
    
    qDebug()<<"RI"<<m_removeInformation->prepare("DELETE FROM information "
                                                 "WHERE idEntry=:ide")<<m_removeInformation->lastError();
    
    qDebug()<<"SI"<<m_selectInformation->prepare("SELECT * FROM information "
                                                 "WHERE idEntry=:ide")<<m_selectInformation->lastError();
}

void ControllerDB::prepareAccount()
{
    m_accounts = SqlQuery::create(m_db);
    m_removeAccount = SqlQuery::create(m_db);
    
    qDebug()<<"SA"<<m_accounts->prepare("SELECT DISTINCT account FROM account WHERE profile=:profile")<<m_accounts->lastError();
    
    //    qDebug()<<"RA"<<m_removeAccount->prepare("DELETE FROM account "
    //                                             "WHERE account=:a AND profile=:profile "
    //                                             "DELETE FROM categories "
    //                                             "WHERE account=:a AND profile=:profile "
    //                                             "DELETE FROM budget "
    //                                             "WHERE account=:a AND profile=:profile "
    //                                             "DELETE FROM frequency "
    //                                             "WHERE account=:a AND profile=:profile "
    //                                             "DELETE FROM commonExpanse "
    //                                             "WHERE account=:a AND profile=:profile ")<<m_removeAccount->lastError();
    
}

void ControllerDB::prepareCategory()
{
    m_addCategory = SqlQuery::create(m_db);
    m_removeCategory = SqlQuery::create(m_db);
    m_selectCategory = SqlQuery::create(m_db);
    
    qDebug()<<"AC"<<m_addCategory->prepare("INSERT INTO categories(name, type, account, profile) "
                                           "VALUES(:name, :type, :account, :profile)")<<m_addCategory->lastError();
    
    qDebug()<<"RC"<<m_removeCategory->prepare("DELETE FROM categories "
                                              "WHERE name = :name AND account=:account AND profile=:profile")<<m_removeCategory->lastError();
    
    qDebug()<<"SC"<<m_selectCategory->prepare("SELECT * FROM categories "
                                              "WHERE account=:account AND profile=:profile")<<m_selectCategory->lastError();
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
    
    qDebug()<<"AB"<<m_addBudget->prepare("INSERT INTO budget (account, category, reference, profile)"
                                         "VALUES (:account, :category, :reference, :profile)")<<m_addBudget->lastError();
    
    qDebug()<<"UB"<<m_updateBudget->prepare("UPDATE budget "
                                            "SET category=:c, reference=:r "
                                            "WHERE (id=:id)")<<m_updateBudget->lastError();
    
    qDebug()<<"RB"<<m_removeBudget->prepare("DELETE FROM budget "
                                            "WHERE id=:id")<<m_removeBudget->lastError();
    
    qDebug()<<"SB"<<m_selectBudget->prepare("SELECT * FROM budget "
                                            "WHERE account=:a AND profile=:profile")<<m_selectBudget->lastError();
    
    qDebug()<<"SSB"<<m_selectSubBudget->prepare("SELECT * FROM subbudget "
                                                "WHERE idBudget=:idb")<<m_selectSubBudget->lastError();
    
    qDebug()<<"USB"<<m_updateSubbudget->prepare("UPDATE subbudget "
                                                "SET frequency=:freq, target=:target "
                                                "WHERE idBudget=:idb AND fromDate=:date")<<m_updateSubbudget->lastError();
    
    qDebug()<<"RSB"<<m_removeSubbudget->prepare("DELETE FROM subbudget "
                                                "WHERE idBudget=:idb AND fromDate=:date")<<m_removeSubbudget->lastError();
    
    qDebug()<<"ASB"<<m_addSubbudget->prepare("INSERT INTO subbudget (idBudget, frequency, target, fromDate) "
                                             "VALUES (:idb, :frequency, :target, :date)")<<m_addSubbudget->lastError();
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
    
    qDebug()<<"SF"<<m_selectFrequency->prepare("SELECT * FROM frequency "
                                               "WHERE account=:a AND profile=:profile")<<m_selectFrequency->lastError();
    
    qDebug()<<"AF"<<m_addFrequency->prepare("INSERT INTO frequency (ID, freq, nbGroup) "
                                            "VALUES(:id, :freq, :nbGroup)")<<m_addFrequency->lastError();
    
    qDebug()<<"RF"<<m_removeFrequency->prepare("DELETE FROM frequency "
                                               "WHERE id=:id" )<<m_removeFrequency->lastError();
    
    qDebug()<<"UF"<<m_updateFrequency->prepare("UPDATE frequency "
                                               "SET freq=:f, nbGroup=:ng "
                                               "WHERE id=:id")<<m_updateFrequency->lastError();
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
    
    qDebug()<<"SP"<< m_selectProfiles->prepare("SELECT DISTINCT profile FROM account")<<m_selectProfiles->lastError();
    
    //    qDebug()<<"RP"<<m_removeProfile->prepare("DELETE FROM account, categories, budget, frequency, commonExpanse "
    //                                             "WHERE profile=:profile")<<m_removeProfile->lastError();
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
        m_addEntry->bindValue(":profile", m_currentProfile);
        
        ret = m_addEntry->exec();
        int id = m_addEntry->lastInsertId().toInt();

        if(ret && id >= 0)
        {
            
            m_addInformation->bindValue(":ide", id);
            m_addInformation->bindValue(":title", e.info().title());
            m_addInformation->bindValue(":prev",e.info().estimated());
            m_addInformation->bindValue(":cat",e.info().category());
            
            ret &= m_addInformation->exec();
        }
    }
    
    return ret;
}

QMultiMap<QDate, Entry> ControllerDB::selectEntry(QString account)
{
    QMultiMap<QDate, Entry> res;
    
    if(!isConnected())
        return res;
    m_currentAccount = account;
    m_selectEntry->bindValue(":a", QVariant(account));
    m_selectEntry->bindValue(":p", m_currentProfile);
    if(m_selectEntry->exec())
    {
        while(m_selectEntry->next())
        {
            if(m_selectEntry->at() < 0)
                continue;
            Entry t;
            Information i;
            t.setId(m_selectEntry->value("id").toInt());
            t.setDate(m_selectEntry->value("date_eff").toDate());
            t.setValue(m_selectEntry->value("value").toDouble());
            t.setType(m_selectEntry->value("type").toString());
            
            m_selectInformation->bindValue(":ide", t.id());
            bool inf = m_selectInformation->exec();
            if(!inf)
                continue;
            
            m_selectInformation->seek(0);
            
            i.setId(m_selectInformation->value("id").toInt());
            i.setIdEntry(m_selectInformation->value("idEntry").toInt());
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
    m_accounts->bindValue(":profile", m_currentProfile);
    if(isConnected() && m_accounts->exec())
    {
        while(m_accounts->next())
            res<<m_accounts->value("account").toString();
    }
    
    return res;
}

bool ControllerDB::updateEntry(const Entry & e)
{
    bool ret = false;
    
    if(isConnected())
    {
        m_updateEntry->bindValue(":v", e.value());
        m_updateEntry->bindValue(":id", e.id());
        
        ret = m_updateEntry->exec();
        
        ret &= updateInfo(e);
    }
    
    
    return ret;
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
        
        m_selectCategory->bindValue(":account", m_currentAccount);
        m_selectCategory->bindValue(":profile", m_currentProfile);
        if(m_selectCategory->exec())
            while(m_selectCategory->next())
                if(e.info().category() == m_selectCategory->value("name").toString())
                    m_updateInfo->bindValue(":cat", m_selectCategory->value("id"));
        
        ret = m_updateInfo->exec();
    }
    
    return ret;
}

bool ControllerDB::removeEntry(const Entry & e)
{
    bool ret = false;
    
    if(isConnected())
    {
        //        m_removeInformation->bindValue(":ide", e.id());
        m_removeEntry->bindValue(":id", e.id());
        
        
        ret = m_removeEntry->exec();
        if(ret)
        {
            auto req = m_db.exec("SELECT id FROM information");

            while(req.next())
                qDebug()<<req.value("id");
        }
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
    if(isConnected() && !n.isEmpty())
    {
        m_addCategory->bindValue(":name", n);
        m_addCategory->bindValue(":type", t);
        m_addCategory->bindValue(":account", m_currentAccount);
        m_addCategory->bindValue(":profile", m_currentProfile);
        
        return m_addCategory->exec();
    }
    
    return false;
}

bool ControllerDB::removeCategory(QString name)
{
    if(isConnected())
    {
        m_removeCategory->bindValue(":name", name);
        m_removeCategory->bindValue(":account", m_currentAccount);
        m_removeCategory->bindValue(":profile", m_currentProfile);
        return m_removeCategory->exec();
    }
    
    return false;
}

QMultiMap<QString, QString> ControllerDB::selectCategory()
{
    QMultiMap<QString, QString> ret;
    m_selectCategory->bindValue(":account", m_currentAccount);
    m_selectCategory->bindValue(":profile", m_currentProfile);
    
    if(isConnected() && m_selectCategory->exec())
    {
        while(m_selectCategory->next())
        {
            QString name = m_selectCategory->value("name").toString();
            QString type = m_selectCategory->value("type").toString();
            
            if(name.isEmpty())
                continue;
            
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
