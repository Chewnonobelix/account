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
    
    if(m_db.tables().isEmpty())
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
        qDebug()<<"PRT"<<m_db.exec(profile_remove_table).lastError();
        qDebug()<<"ART"<<m_db.exec(account_remove_table).lastError();
        
        qDebug()<<"Trigger delete entry"<<m_db.exec(account_trigger_delete).lastError();
        qDebug()<<"Trigger delete frequency"<<m_db.exec(trigger_delete_frequency).lastError();
        qDebug()<<"Trigger update budget"<<m_db.exec(budget_trigger).lastError();
        qDebug()<<"Trigger delete budget"<<m_db.exec(budget_delete_trigger).lastError();
        qDebug()<<"Trigger delete category"<<m_db.exec(trigger_delete_category).lastError();
        qDebug()<<"Trigger delete common expanse"<<m_db.exec(trigger_delete_commonExpanse).lastError();
        qDebug()<<"Trigger update common expanse"<<m_db.exec(expanse_trigger).lastError();
        qDebug()<<"Trigger delete common entry"<<m_db.exec(trigger_delete_commonEntry).lastError();
        qDebug()<<"Trigger delete account"<<m_db.exec(remove_account_trigger).lastError();
        qDebug()<<"Trigger delete profile"<<m_db.exec(remove_profile_trigger).lastError();
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
                                           "WHERE account=:a AND profile=:p AND frequencyReference IS NULL AND "
                                           "id NOT IN (SELECT entry FROM commonEntry) ")<<m_selectEntry->lastError();
    
    qDebug()<<"AE"<<m_addEntry->prepare("INSERT INTO account (account, value, date_eff, type, profile) "
                                        "VALUES (:account,:value,:date,:type, :profile)")<<m_addEntry->lastError();
    
    qDebug()<<"RE"<<m_removeEntry->prepare("DELETE FROM account "
                                           "WHERE id=:id")<<m_removeEntry->lastError();
    
    qDebug()<<"UE"<<m_updateEntry->prepare("UPDATE account "
                                           "SET value=:v, type=:t "
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
    
    qDebug()<<"RA"<<m_removeAccount->prepare("INSERT INTO temp_account VALUES (:n)")<<m_removeAccount->lastError();    
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
                                                "WHERE idBudget=:idb")<<m_removeSubbudget->lastError();
    
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
    
    qDebug()<<"SFR"<<m_selectFrequencyReference->prepare("SELECT * FROM account "
                                                         "WHERE frequencyReference=:f")<<m_selectFrequency->lastError();
    
    qDebug()<<"AF"<<m_addFrequency->prepare("INSERT INTO frequency (freq, nbGroup, account, profile) "
                                            "VALUES (:freq, :nbGroup, :account, :profile)")<<m_addFrequency->lastError();
    
    qDebug()<<"AFR"<<m_addFrequencyReference->prepare("INSERT INTO account (account, value, type, date_eff, profile, frequencyReference) "
                                                      "VALUES(:a, :v, :t, :d, :p, :f)")<<m_addFrequencyReference->lastError();
    
    qDebug()<<"RF"<<m_removeFrequency->prepare("DELETE FROM frequency "
                                               "WHERE id=:id" )<<m_removeFrequency->lastError();
    
    qDebug()<<"UF"<<m_updateFrequency->prepare("UPDATE frequency "
                                               "SET freq=:f, nbGroup=:ng "
                                               "WHERE id=:id")<<m_updateFrequency->lastError();
}

void ControllerDB::prepareCommon()
{
    m_addCommon = SqlQuery::create(m_db);
    m_addCommonEntry = SqlQuery::create(m_db);
    m_selectCommon = SqlQuery::create(m_db);
    m_removeCommon = SqlQuery::create(m_db);
    m_updateCommon = SqlQuery::create(m_db);
    m_addCommonTable = SqlQuery::create(m_db);
    m_selectCommonTable = SqlQuery::create(m_db);
    m_selectCommonEntry = SqlQuery::create(m_db);
    m_addCommonEntryInformation = SqlQuery::create(m_db);
    
    qDebug()<<"ACEX"<<m_addCommon->prepare("INSERT INTO commonExpanse (begin, isClose, title, profile, account) "
                                           "VALUES (:b, :c, :t, :p, :a)")<<m_addCommon->lastError();
    
    qDebug()<<"SCEX"<<m_selectCommon->prepare("SELECT * FROM commonExpanse "
                                              "WHERE account=:a AND profile=:p")<<m_selectCommon->lastError();
    
    qDebug()<<"DCEX"<<m_removeCommon->prepare("DELETE FROM commonExpanse "
                                              "WHERE id=:id")<<m_removeCommon->lastError();
    
    qDebug()<<"UCEX"<<m_updateCommon->prepare("UPDATE commonExpanse "
                                              "SET isClose=:c "
                                              "WHERE id=:id")<<m_updateCommon->lastError();
    
    qDebug()<<"ACEN"<<m_addCommonEntry->prepare("INSERT INTO account (account, profile, value, date_eff, type) "
                                                "VALUES (:a, :p, :v, :d, :t)")<<m_addCommonEntry->lastError();
        
    qDebug()<<"ACENI"<<m_addCommonEntryInformation->prepare("INSERT INTO information (idEntry, info) "
                                                "VALUES (:ide, :title)")<<m_addCommonEntry->lastError();
    
    qDebug()<<"SCEN"<<m_selectCommonEntry->prepare("SELECT * FROM account "
                                                   "WHERE id=:id")<<m_selectCommonEntry->lastError();
    
    qDebug()<<"ACET"<<m_addCommonTable->prepare("INSERT INTO commonEntry (idCommon, entry, name) "
                                                "VALUES (:i, :e, :n)")<<m_addCommonTable->lastError();
    
    qDebug()<<"SCET"<<m_selectCommonTable->prepare("SELECT * FROM commonEntry "
                                                   "")<<m_selectCommonTable->lastError();
}

void ControllerDB::prepareProfile()
{
    m_selectProfiles = SqlQuery::create(m_db);
    m_removeProfile = SqlQuery::create(m_db);
    
    qDebug()<<"SP"<< m_selectProfiles->prepare("SELECT DISTINCT profile FROM account")<<m_selectProfiles->lastError();
    
    qDebug()<<"RP"<<m_removeProfile->prepare("INSERT INTO temp_profile VALUES (:n)")<<m_removeProfile->lastError();
}

bool ControllerDB::isConnected() const
{
    return m_db.isOpen();
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

bool ControllerDB::removeAccount(QString name)
{
    bool ret = false;
    
    if(isConnected())
    {
        m_removeAccount->bindValue(":n", name);
        ret = m_removeAccount->exec();
    }
    
    return ret;
}

QStringList ControllerDB::selectProfile() 
{
    QStringList ret;
    if(isConnected())
    {
        m_selectProfiles->exec();
        while(m_selectProfiles->next())
            ret<<m_selectProfiles->value("profile").toString();
    }
    
    return ret;
}

void ControllerDB::setProfile(QString p)
{
    m_currentProfile = p;
}

bool ControllerDB::addProfile(QString name, QString) 
{
    bool ret = !selectProfile().contains(name);
    
    if(ret)
        m_currentProfile = name;
    
    return ret;
}

QString ControllerDB::currentProfile()
{
    return m_currentProfile;
}

bool ControllerDB::deleteProfile(QString name) 
{
    if(isConnected())
    {
        m_removeProfile->addBindValue(name);
        bool ret = m_removeProfile->exec();
        
        return ret;
    }
    
    return false;
}

bool ControllerDB::addEntry(const Entry & e)
{        
    bool ret = false;
    if(isConnected() && e.id() < 0)
    {
        
        m_addEntry->bindValue(":account", e.account().isEmpty() ? m_currentAccount : e.account());
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
            
            ret &= m_addInformation->exec();
        }
        
        auto meta = e.metaDataList();
        for(auto it: meta)
        {
            if(it == "id")
                continue;
            
            m_insertMetadata->bindValue(":entry", id);
            m_insertMetadata->bindValue(":name", it);
            m_insertMetadata->bindValue(":value", e.metaData<QString>(it));
            
            ret &= m_insertMetadata->exec();
        }
    }
    
    if(e.id() != -2)
        emit s_updateEntry();

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
            
            auto reqc = m_db.exec("SELECT name FROM categories WHERE id='"+QString::number(m_selectInformation->value("category").toInt())+"'");
            
            if(reqc.seek(0))
                i.setCategory(reqc.value("name").toString());
            
            t.setInfo(i);
            
            m_selectMetadata->bindValue(":ide", t.id());
            
            if(m_selectMetadata->exec())
                while(m_selectMetadata->next())
                    t.setMetadata(m_selectMetadata->value("name").toString(), m_selectMetadata->value("value").toString());
            
            res.insert(t.date(), t);
        }
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
        m_updateEntry->bindValue(":t", e.type());
        
        ret = m_updateEntry->exec();
        
        ret &= updateInfo(e);
        
        
        auto meta = e.metaDataList();
        for(auto it: meta)
        {
            if(it == "id")
                continue;
            
            m_updateMetadata->bindValue(":entry", e.id());
            m_updateMetadata->bindValue(":name", it);
            m_updateMetadata->bindValue(":value", e.metaData<QString>(it));
            
            bool t = m_updateMetadata->exec();
            t = m_updateMetadata->numRowsAffected() > 0;
            if(!t)
            {
                m_insertMetadata->bindValue(":entry", e.id());
                m_insertMetadata->bindValue(":name", it);
                m_insertMetadata->bindValue(":value", e.metaData<QString>(it));
                
                m_insertMetadata->exec();                
            }
        }
    }
    
    emit s_updateEntry(e.id());    
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
        
        auto reqc = m_db.exec("SELECT id FROM categories WHERE account='"+m_currentAccount+"' AND profile='"+m_currentProfile+"' AND name='"+e.info().category()+"'");
        
        if(reqc.seek(0))
            m_updateInfo->bindValue(":cat", reqc.value("id").toInt());
        
        ret = m_updateInfo->exec();
    }
    
    return ret;
}

bool ControllerDB::removeEntry(const Entry & e)
{
    bool ret = false;
    
    if(isConnected())
    {
        m_removeEntry->bindValue(":id", e.id());
        
        ret = m_removeEntry->exec();
    }
    
    emit s_updateEntry();
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
        
        emit s_updateCategory();
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
        emit s_updateCategory();
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

bool ControllerDB::addBudget(const Budget& b)
{
    if(isConnected())
    {
        m_addBudget->bindValue(":account", m_currentAccount);
        m_addBudget->bindValue(":profile", m_currentProfile);
        m_addBudget->bindValue(":reference", b.reference());
        auto reqc = m_db.exec("SELECT * FROM categories WHERE profile='"+m_currentProfile+"' AND account='"+m_currentAccount+"' AND name='"+b.category()+"' ");
        
        reqc.seek(0);
        
        int idc = reqc.value("id").toInt();
        m_addBudget->bindValue(":category", idc);
        
        emit s_updateBudget();
        return m_addBudget->exec();
    }
    
    return false;
}

bool ControllerDB::removeBudget(const Budget & b)
{
    if(isConnected())
    {
        m_removeBudget->bindValue(":id", b.id());
        emit s_updateBudget();
        return m_removeBudget->exec();
    }
    
    return false;
}

QList<Budget> ControllerDB::selectBudgets()
{
    QList<Budget> ret;
    
    if(isConnected())
    {
        m_selectBudget->bindValue(":a", m_currentAccount);
        m_selectBudget->bindValue(":profile", m_currentProfile);
        
        m_selectBudget->exec();
        
        while(m_selectBudget->next())
        {
            Budget b;
            b.setId(m_selectBudget->value("id").toInt());
            b.setReference(m_selectBudget->value("reference").toDate());
            auto req = m_db.exec("SELECT name FROM categories WHERE id='"+m_selectBudget->value("category").toString()+"'");
            req.seek(0);
            b.setCategory(req.value("name").toString());
            
            m_selectSubBudget->bindValue(":idb", b.id());
            m_selectSubBudget->exec();
            
            while(m_selectSubBudget->next())
            {
                b.addTarget(m_selectSubBudget->value("fromDate").toDate(), m_selectSubBudget->value("target").toDouble());    
                b.setFrequency(m_selectSubBudget->value("fromDate").toDate(), (Account::FrequencyEnum)m_selectSubBudget->value("frequency").toInt());    
            }
            
            ret<<b;
        }
    }
    return ret;
}

bool ControllerDB::updateBudget(const Budget & b)
{
    if(isConnected())
    {
        for(auto it: b.targets().keys())
        {
            m_addSubbudget->bindValue(":idb", b.id());
            m_addSubbudget->bindValue(":date", it);
            m_addSubbudget->bindValue(":frequency", (int)b.frequency(it));
            m_addSubbudget->bindValue(":target", b.targets()[it]);
            
            m_addSubbudget->exec();
        }
        emit s_updateBudget();        
    }
    
    return false;
}

bool ControllerDB::addFrequency(const Frequency & f)
{
    if(isConnected())
    {
        m_addFrequency->bindValue(":freq", QVariant::fromValue((int)f.freq()));
        m_addFrequency->bindValue(":nbGroup", QVariant::fromValue(f.nbGroup()));
        m_addFrequency->bindValue(":account", QVariant::fromValue(m_currentAccount));
        m_addFrequency->bindValue(":profile", QVariant::fromValue(m_currentProfile));
        
        if(m_addFrequency->exec())
        {
            int id = m_addFrequency->lastInsertId().toInt();
            m_addFrequencyReference->bindValue(":a", m_currentAccount);
            m_addFrequencyReference->bindValue(":v", f.referenceEntry().value());
            m_addFrequencyReference->bindValue(":t", f.referenceEntry().type());
            m_addFrequencyReference->bindValue(":d", f.referenceEntry().date());
            m_addFrequencyReference->bindValue(":p", m_currentProfile);
            m_addFrequencyReference->bindValue(":f", id);
            
            if(m_addFrequencyReference->exec())
            {
                id = m_addFrequencyReference->lastInsertId().toInt();
                Information i = f.referenceEntry().info();
                m_addInformation->bindValue(":ide", id);
                m_addInformation->bindValue(":title", i.title());
                m_addInformation->bindValue(":prev",i.estimated());
                
                bool ret = m_addInformation->exec();
                
                emit s_updateFrequency();
                return ret;
            }
            
        }
    }
    return false;
}

bool ControllerDB::removeFrequency(const Frequency& f)
{
    if(isConnected())
    {
        m_removeFrequency->bindValue(":id", f.id());
        emit s_updateFrequency();
        return m_removeFrequency->exec();
    }
    
    return false;
}

bool ControllerDB::updateFrequency(const Frequency& f)
{
    if(isConnected())
    {
        bool ret = updateEntry(f.referenceEntry());
        m_updateFrequency->bindValue(":f", (int)f.freq());
        m_updateFrequency->bindValue(":ng", f.nbGroup());
        m_updateFrequency->bindValue(":id", f.id());
        
        emit s_updateFrequency();
        return ret && m_updateFrequency->exec();
    }
    return false;
}

QList<Frequency> ControllerDB::selectFrequency()
{
    QList<Frequency> ret;
    
    if(isConnected())
    {
        m_selectFrequency->bindValue(":a", m_currentAccount);
        m_selectFrequency->bindValue(":profile", m_currentProfile);
        
        m_selectFrequency->exec();
        
        while(m_selectFrequency->next())
        {
            Frequency f;
            f.setId(m_selectFrequency->value("id").toInt());
            f.setFreq((Account::FrequencyEnum)m_selectFrequency->value("freq").toInt());
            f.setNbGroup(m_selectFrequency->value("nbGroup").toInt());
            
            m_selectFrequencyReference->bindValue(":f", f.id());
            
            if(!m_selectFrequencyReference->exec())
                continue;
            
            m_selectFrequencyReference->seek(0);
            Entry ref;
            Information i;
            ref.setId(m_selectFrequencyReference->value("id").toInt());
            ref.setDate(m_selectFrequencyReference->value("date_eff").toDate());
            ref.setValue(m_selectFrequencyReference->value("value").toDouble());
            ref.setType(m_selectFrequencyReference->value("type").toString());
            
            m_selectInformation->bindValue(":ide", ref.id());
            
            bool inf = m_selectInformation->exec();
            if(!inf)
                continue;
            
            
            m_selectInformation->seek(0);
            
            i.setId(m_selectInformation->value("id").toInt());
            i.setIdEntry(m_selectInformation->value("idEntry").toInt());
            i.setEstimated(m_selectInformation->value("prev").toBool());
            i.setTitle(m_selectInformation->value("info").toString());
            
            ref.setInfo(i);
            
            m_selectMetadata->bindValue(":ide", ref.id());
            
            if(m_selectMetadata->exec())
                while(m_selectMetadata->next())
                    ref.setMetadata(m_selectMetadata->value("name").toString(), m_selectMetadata->value("value").toString());
            
            f.setReferenceEntry(ref);
            ret<<f;
        }
        
    }
    return ret;
}

QMap<int, CommonExpanse> ControllerDB::selectCommon() 
{
    QMap<int, CommonExpanse> ret;
    
    if(isConnected())
    {
        m_selectCommon->bindValue(":a", m_currentAccount);    
        m_selectCommon->bindValue(":p", m_currentProfile);
        
        m_selectCommon->exec();
        
        while(m_selectCommon->next())
        {
            CommonExpanse c;
            c.setId(m_selectCommon->value("id").toInt());
            c.setBegin(m_selectCommon->value("begin").toDate());
            c.setIsClose(m_selectCommon->value("isClose").toBool());
            c.setTitle(m_selectCommon->value("title").toString());
            
            m_selectCommonTable->bindValue(":idc", c.id());
            m_selectCommonTable->exec();
            
            while(m_selectCommonTable->next())
            {
                m_selectCommonEntry->bindValue(":id", m_selectCommonTable->value("entry").toInt());
                m_selectCommonEntry->exec();
                
                if(!m_selectCommonEntry->seek(0))
                    continue;
                
                
                Entry e;
                e.setId(m_selectCommonEntry->value("id").toInt());
                e.setValue(m_selectCommonEntry->value("value").toDouble());
                e.setDate(m_selectCommonEntry->value("date_eff").toDate());
                e.setType(m_selectCommonEntry->value("type").toString());

                m_selectInformation->bindValue(":ide", e.id());
                m_selectInformation->exec();
                
                if(!m_selectInformation->seek(0))
                    continue;
                
                Information i;
                i.setIdEntry(e.id());
                i.setTitle(m_selectInformation->value("info").toString());
                e.setInfo(i);
                
                c.addEntries(m_selectCommonTable->value("name").toString(), e);
            }
            
            ret[c.id()] = c;
        }
    }
    
    return  ret; 
}

bool ControllerDB::addCommon(const CommonExpanse& c)
{
    if(isConnected())
    {
        m_addCommon->bindValue(":b", c.begin());
        m_addCommon->bindValue(":c", c.isClose());
        m_addCommon->bindValue(":t", c.title());
        m_addCommon->bindValue(":p", m_currentProfile);
        m_addCommon->bindValue(":a", m_currentAccount);
        
        emit s_updateCommon();
        return m_addCommon->exec();
    }
    
    return false;
}

bool ControllerDB::removeCommon(const CommonExpanse& c) 
{
    if(isConnected())
    {
        m_removeCommon->bindValue(":id", c.id());
        emit s_updateCommon();
        return m_removeCommon->exec();
    }
    
    return false;
}

bool ControllerDB::updateCommon(const CommonExpanse& c) 
{
    if(isConnected())
    {
        m_updateCommon->bindValue(":id", c.id());
        m_updateCommon->bindValue(":c", c.isClose());
        m_updateCommon->exec();
        
        auto map = c.entries();
        for(auto it = map.begin(); it != map.end(); it++)
        {
            Entry e = *it;
            m_addCommonEntry->bindValue(":a", m_currentAccount);
            m_addCommonEntry->bindValue(":p", m_currentProfile);
            m_addCommonEntry->bindValue(":v", e.value());
            m_addCommonEntry->bindValue(":d",e.date());
            m_addCommonEntry->bindValue(":t", e.type());
            m_addCommonEntry->exec();
            
            int id = m_addCommonEntry->lastInsertId().toInt();
            m_addCommonEntryInformation->bindValue(":ide", id);
            m_addCommonEntryInformation->bindValue(":title", e.info().title());
            
            m_addCommonEntryInformation->exec();
                        
            m_addCommonTable->bindValue(":i", c.id());
            m_addCommonTable->bindValue(":e", id);
            m_addCommonTable->bindValue(":n", it.key());
            
            m_addCommonTable->exec();
        }
        
        emit s_updateCommon();
        return true;
    }
    
    return false;
}
