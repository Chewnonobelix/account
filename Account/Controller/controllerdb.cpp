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
        qDebug()<<"PRT"<<m_db.exec(profile_remove_table).lastError();
        qDebug()<<"ART"<<m_db.exec(account_remove_table).lastError();
        
        qDebug()<<"Trigger delete entry"<<m_db.exec(account_trigger_delete).lastError();
        qDebug()<<"Trigger delete frequency"<<m_db.exec(trigger_delete_frequency).lastError();
        qDebug()<<"Trigger delete category"<<m_db.exec(trigger_delete_category).lastError();
        qDebug()<<"Trigger delete common expanse"<<m_db.exec(trigger_delete_commonExpanse).lastError();
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
        
        qDebug()<<selectProfile();
        return ret;
    }
    
    return false;
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
        m_removeEntry->bindValue(":id", e.id());
        
        ret = m_removeEntry->exec();
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

                return ret;
            }
            
        }
        qDebug()<<"add frequency"<<m_addFrequency->lastError()<<m_addFrequencyReference->lastError()<<m_addInformation->lastError();
    }
    return false;
}

bool ControllerDB::removeFrequency(const Frequency& f)
{
    if(isConnected())
    {
        m_removeFrequency->bindValue(":id", f.id());
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

        return ret && m_updateFrequency->exec();
    }
    return false; //TODO
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
            f.setEnd(m_selectFrequency->value("end").toDate());

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

QMap<int, CommonExpanse> ControllerDB::selectCommon() {return  QMap<int, CommonExpanse>(); }

bool ControllerDB::addCommon(const CommonExpanse&) {return false;}

bool ControllerDB::removeCommon(const CommonExpanse&) {return false;}

bool ControllerDB::updateCommon(const CommonExpanse&) {return false;}
