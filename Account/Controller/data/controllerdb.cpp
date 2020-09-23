#include "controllerdb.h"
#include "dbrequestsinit.h"
#include <QDebug>
#include <QDir>

ControllerDB::ControllerDB(bool backup)
{
    this->backup = backup;
    m_currentProfile = "Default";
}

ControllerDB::ControllerDB(const ControllerDB& d): InterfaceDataSave(d)
{
    m_currentProfile = d.m_currentProfile;
}

ControllerDB::~ControllerDB()
{
    if(isConnected())
    {
        m_db.close();
    }
}

bool ControllerDB::init()
{
    if(!backup && m_db.connectionNames().contains("default"))
    {
        m_db.database("default").close();
        m_db.removeDatabase("default");
    }
    
    if(backup && m_db.connectionNames().contains("backup"))
    {
        m_db.database("backup").close();
        m_db.removeDatabase("backup");
    }
    
    
    m_db = QSqlDatabase::addDatabase("QSQLITE", backup ? "backup" : "default");
    qDebug()<<"Back?"<<backup<<m_db.connectionNames()<<m_db.connectionName();
    QString name = QString("account%1").arg(backup ? "_backup" : "");
    
    m_db.setDatabaseName(name);
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
        qDebug() << "DT" << m_db.exec(debt_table).lastError();

        qDebug() << "Trigger delete entry" << m_db.exec(account_trigger_delete).lastError();
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
        prepareDebt();
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
    
    qDebug()<<"AE"<<m_addEntry->prepare("INSERT INTO account (id, account, value, date_eff, type, profile) "
                                        "VALUES (:id, :account,:value,:date,:type, :profile)")<<m_addEntry->lastError();
    
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
    
    qDebug()<<"AI"<<m_addInformation->prepare("INSERT INTO information (id, idEntry, info, prev, category) "
                                              "VALUES(:id, :ide, :title, :prev, :cat)")<<m_addInformation->lastError();
    
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
    
    qDebug()<<"AB"<<m_addBudget->prepare("INSERT INTO budget (id, account, category, reference, profile)"
                                         "VALUES (:id, :account, :category, :reference, :profile)")<<m_addBudget->lastError();
    
    qDebug()<<"UB"<<m_updateBudget->prepare("UPDATE budget "
                                            "SET category=:c, reference=:r, removed=:rm "
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
    
    qDebug()<<"AF"<<m_addFrequency->prepare("INSERT INTO frequency (id, freq, nbGroup, account, profile) "
                                            "VALUES (:id, :freq, :nbGroup, :account, :profile)")<<m_addFrequency->lastError();
    
    qDebug()<<"AFR"<<m_addFrequencyReference->prepare("INSERT INTO account (account, value, type, date_eff, profile, frequencyReference) "
                                                      "VALUES(:a, :v, :t, :d, :p, :f)")<<m_addFrequencyReference->lastError();
    
    qDebug()<<"RF"<<m_removeFrequency->prepare("DELETE FROM frequency "
                                               "WHERE id=:id" )<<m_removeFrequency->lastError();
    
    qDebug()<<"UF"<<m_updateFrequency->prepare("UPDATE frequency "
                                               "SET freq=:f, nbGroup=:ng, endless=:el, removed=:r "
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
    
    qDebug()<<"ACEX"<<m_addCommon->prepare("INSERT INTO commonExpanse (id, begin, isClose, title, profile, account) "
                                           "VALUES (:i, :b, :c, :t, :p, :a)")<<m_addCommon->lastError();
    
    qDebug()<<"SCEX"<<m_selectCommon->prepare("SELECT * FROM commonExpanse "
                                              "WHERE account=:a AND profile=:p")<<m_selectCommon->lastError();
    
    qDebug()<<"DCEX"<<m_removeCommon->prepare("DELETE FROM commonExpanse "
                                              "WHERE id=:id")<<m_removeCommon->lastError();
    
    qDebug()<<"UCEX"<<m_updateCommon->prepare("UPDATE commonExpanse "
                                              "SET isClose=:c, removed=:r "
                                              "WHERE id=:id")<<m_updateCommon->lastError();
    
    qDebug()<<"ACEN"<<m_addCommonEntry->prepare("INSERT INTO account (id, account, profile, value, date_eff, type) "
                                                "VALUES (:id, :a, :p, :v, :d, :t)")<<m_addCommonEntry->lastError();
    
    qDebug()<<"ACENI"<<m_addCommonEntryInformation->prepare("INSERT INTO information (id, idEntry, info) "
                                                            "VALUES (:id, :ide, :title)")<<m_addCommonEntry->lastError();
    
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

void ControllerDB::prepareDebt()
{
    m_selectDebt = SqlQuery::create(m_db);
    m_addDebt = SqlQuery::create(m_db);
    m_removeDebt = SqlQuery::create(m_db);
    m_updateDebt = SqlQuery::create(m_db);

    qDebug() << "SD"
             << m_selectDebt->prepare(QStringLiteral(
                    "SELECT * FROM debt WHERE account=:account AND profile=:profile"))
             << m_selectDebt->lastError();
    qDebug() << "AD"
             << m_addDebt->prepare(
                    QStringLiteral("INSERT INTO debt (id, freq, nb, rate, name, account, profile) "
                                   "VALUES (:id, :freq, :nb, :rate, :name, :account, :profile)"))
             << m_addDebt->lastError();
    qDebug() << "UD"
             << m_updateDebt->prepare(QStringLiteral(
                    "UPDATE debt "
                    "SET freq=:freq, nb=:nb, rate=:rate, name=:name, removed=:removed "
                    "WHERE account=:account AND profile=:profile "))
             << m_updateDebt->lastError();
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

void ControllerDB::setCurrentAccount(QString account)
{
    m_currentAccount = account;
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
    
    if(isConnected())
    {
        m_addEntry->bindValue(":account", e.account().isEmpty() ? m_currentAccount : e.account());
        m_addEntry->bindValue(":value", QVariant(e.value()));
        m_addEntry->bindValue(":date", QVariant(e.date()));
        m_addEntry->bindValue(":type", QVariant(e.type().toLower()));
        m_addEntry->bindValue(":profile", m_currentProfile);
        Entry et = e;
        QUuid id = e.id().isNull() ? QUuid::createUuid() : e.id();
        m_addEntry->bindValue(":id", id);
        et.setId(id);

        qDebug() << et.title() << et.metadataList();
        ret = m_addEntry->exec();
        
        if(ret && id >= 0)
        {
            
            m_addInformation->bindValue(":ide", id);
            m_addInformation->bindValue(":title", e.title());
            m_addInformation->bindValue(":prev", e.estimated());
            m_addInformation->bindValue(":cat", -1);
            id = e.id().isNull() ? QUuid::createUuid() : e.id();
            m_addInformation->bindValue(":id", id);
            
            ret &= m_addInformation->exec();
        }

        et.setMetadata("lastUpdate", QDateTime::currentDateTime());

        auto meta = et.metadataList();
        for(auto it: meta)
        {
            if (QStringList({"id", "date", "value", "account", "type", "notemit"}).contains(it))
                continue;

            qDebug() << it
                     << QStringList({"id", "date", "value", "account", "type", "notemit"})
                            .contains(it)
                     << et.metaData<QString>(it);
            m_insertMetadata->bindValue(":entry", id);
            m_insertMetadata->bindValue(":name", it);
            m_insertMetadata->bindValue(":value", et.metaData<QString>(it));

            ret &= m_insertMetadata->exec();
        }
    }
    
    if(!e.hasMetadata("notemit"))
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
            if(m_selectEntry->at() < 0 || !m_selectEntry->isValid())
                continue;
            
            
            Entry t;
            t.setId(m_selectEntry->value("id").toUuid());
            t.setDate(m_selectEntry->value("date_eff").toDate());
            t.setValue(m_selectEntry->value("value").toDouble());
            t.setType(m_selectEntry->value("type").toString().toLower());
            t.setAccount(m_selectEntry->value("account").toString());

            m_selectInformation->bindValue(":ide", t.id());
            bool inf = m_selectInformation->exec();
            if(!inf)
                continue;

            m_selectInformation->seek(0);

            t.setEstimated(m_selectInformation->value("prev").toBool());
            t.setTitle(m_selectInformation->value("info").toString());

            auto reqc = m_db.exec("SELECT name FROM categories WHERE id='"
                                  + QString::number(m_selectInformation->value("category").toInt())
                                  + "'");

            if (reqc.seek(0))
                t.setCategory(reqc.value("name").toString());

            m_selectMetadata->bindValue(":ide", t.id());
            bool mt = m_selectMetadata->exec();
            if(!mt)
                continue;

            //            qDebug() << "sdqsdqsdqsd--------";
            while (m_selectMetadata->next()) {
                qDebug() << "pouet" << m_selectMetadata->value("name").toString()
                         << m_selectMetadata->value("value").toString();
                t.setMetadata(m_selectMetadata->value("name").toString(),
                              m_selectMetadata->value("value").toString());
            }

            if (t.metaData<QString>("removed") == "true")
                continue;
            
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
                
        
        Entry et = e;
        et.setMetadata("lastUpdate", QDateTime::currentDateTime());

        auto meta = et.metadataList();
        for(auto it: meta)
        {
            if (!QStringList({"id", "date", "value", "account", "type"}).contains(it)
                || it == "notemit")
                continue;
            
            m_updateMetadata->bindValue(":entry", et.id());
            m_updateMetadata->bindValue(":name", it);
            m_updateMetadata->bindValue(":value", et.metaData<QString>(it));
            
            bool t = m_updateMetadata->exec();
            t = m_updateMetadata->numRowsAffected() > 0;
            if(!t)
            {
                m_insertMetadata->bindValue(":entry", et.id());
                m_insertMetadata->bindValue(":name", it);
                m_insertMetadata->bindValue(":value", et.metaData<QString>(it));
                
                m_insertMetadata->exec();                
            }
        }
    }
    
    emit s_updateEntry(e.id());    
    return ret;
}

bool ControllerDB::removeEntry(const Entry & e)
{
    bool ret = false;
    
    if(isConnected())
    {
        Entry t(e);
        t.setMetadata("removed", true);
        ret = updateEntry(t);
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
        QUuid idb = b.id().isNull() ? QUuid::createUuid() : b.id();
        m_addBudget->bindValue(":id", idb);
        
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
        Budget t(b);
        t.setMetadata("removed", true);
        return updateBudget(t);
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
            if(m_selectBudget->value("removed").toBool())
                continue;
            
            Budget b;
            b.setId(m_selectBudget->value("id").toUuid());
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
        bool ret = true;
        m_updateBudget->bindValue(":c", b.category());
        m_updateBudget->bindValue(":rm", b.metaData<bool>("removed"));
        m_updateBudget->bindValue(":r", b.reference());
        m_updateBudget->bindValue(":id", b.id());
        
        ret &= m_updateBudget->exec();
        for(auto it: b.targets().keys())
        {
            m_addSubbudget->bindValue(":idb", b.id());
            m_addSubbudget->bindValue(":date", it);
            m_addSubbudget->bindValue(":frequency", (int)b.frequency(it));
            m_addSubbudget->bindValue(":target", b.targets()[it]);
            
            ret &= m_addSubbudget->exec();
        }
        emit s_updateBudget();
        return ret;        
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
        QUuid id = f.id().isNull() ? QUuid::createUuid() : f.id();
        m_addFrequency->bindValue(":id", id);
        
        if(m_addFrequency->exec())
        {
            m_addFrequencyReference->bindValue(":a", m_currentAccount);
            m_addFrequencyReference->bindValue(":v", f.referenceEntry().value());
            m_addFrequencyReference->bindValue(":t", f.referenceEntry().type());
            m_addFrequencyReference->bindValue(":d", f.referenceEntry().date());
            m_addFrequencyReference->bindValue(":p", m_currentProfile);
            m_addFrequencyReference->bindValue(":f", id);
            
            if(m_addFrequencyReference->exec())
            {
                m_addInformation->bindValue(":ide", id);
                m_addInformation->bindValue(":title", f.referenceEntry().title());
                m_addInformation->bindValue(":prev", f.referenceEntry().estimated());

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
        Frequency t(f);
        t.setMetadata("removed", true);
        
        return updateFrequency(t);
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
        m_updateFrequency->bindValue(":el", f.endless());
        m_updateFrequency->bindValue(":r", f.metaData<bool>("removed"));
        
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
            if(m_selectFrequency->value("removed").toBool())
                continue;
            
            Frequency f;
            f.setId(m_selectFrequency->value("id").toUuid());
            f.setFreq((Account::FrequencyEnum)m_selectFrequency->value("freq").toInt());
            f.setNbGroup(m_selectFrequency->value("nbGroup").toInt());
            f.setEndless(m_selectFrequency->value("endless").toInt());
            m_selectFrequencyReference->bindValue(":f", f.id());
            
            if(!m_selectFrequencyReference->exec())
                continue;
            
            m_selectFrequencyReference->seek(0);
            Entry ref;

            ref.setId(m_selectFrequencyReference->value("id").toUuid());
            ref.setDate(m_selectFrequencyReference->value("date_eff").toDate());
            ref.setValue(m_selectFrequencyReference->value("value").toDouble());
            ref.setType(m_selectFrequencyReference->value("type").toString());
            
            m_selectInformation->bindValue(":ide", ref.id());
            
            bool inf = m_selectInformation->exec();
            if(!inf)
                continue;

            m_selectInformation->seek(0);

            ref.setEstimated(m_selectInformation->value("prev").toBool());
            ref.setTitle(m_selectInformation->value("info").toString());

            m_selectMetadata->bindValue(":ide", ref.id());

            if (m_selectMetadata->exec())
                while(m_selectMetadata->next())
                    ref.setMetadata(m_selectMetadata->value("name").toString(), m_selectMetadata->value("value").toString());
            
            f.setReferenceEntry(ref);
            
            for(auto it: selectEntry(m_currentAccount))
                f<<it;
            ret<<f;
        }
        
    }
    return ret;
}

QMap<QUuid, CommonExpanse> ControllerDB::selectCommon()
{
    QMap<QUuid, CommonExpanse> ret;
    
    if(isConnected())
    {
        m_selectCommon->bindValue(":a", m_currentAccount);    
        m_selectCommon->bindValue(":p", m_currentProfile);
        
        m_selectCommon->exec();
        
        while(m_selectCommon->next())
        {
            if(m_selectCommon->value("removed").toBool())
                continue;
            
            CommonExpanse c;
            c.setId(m_selectCommon->value("id").toUuid());
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
                e.setId(m_selectCommonEntry->value("id").toUuid());
                e.setValue(m_selectCommonEntry->value("value").toDouble());
                e.setDate(m_selectCommonEntry->value("date_eff").toDate());
                e.setType(m_selectCommonEntry->value("type").toString());
                
                m_selectInformation->bindValue(":ide", e.id());
                m_selectInformation->exec();
                
                if(!m_selectInformation->seek(0))
                    continue;

                e.setTitle(m_selectInformation->value("info").toString());

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

        m_addCommon->bindValue(":i", c.id().isNull() ? QUuid::createUuid() : c.id());
        
        emit s_updateCommon();
        return m_addCommon->exec();
    }
    
    return false;
}

bool ControllerDB::removeCommon(const CommonExpanse& c) 
{
    if(isConnected())
    {
        CommonExpanse t(c);
        t.setMetadata("removed", true);
        
        return updateCommon(t);
    }
    
    return false;
}

bool ControllerDB::updateCommon(const CommonExpanse& c) 
{
    if(isConnected())
    {
        m_updateCommon->bindValue(":id", c.id());
        m_updateCommon->bindValue(":c", c.isClose());
        m_updateCommon->bindValue(":r", c.metaData<bool>("removed"));
        m_updateCommon->exec();
        
        auto map = c.entries();
        for(auto it = map.begin(); it != map.end(); it++)
        {
            Entry e = *it;
            e.setId(QUuid::createUuid());
            m_addCommonEntry->bindValue(":a", m_currentAccount);
            m_addCommonEntry->bindValue(":p", m_currentProfile);
            m_addCommonEntry->bindValue(":v", e.value());
            m_addCommonEntry->bindValue(":d",e.date());
            m_addCommonEntry->bindValue(":t", e.type());
            m_addCommonEntry->bindValue(":id", e.id());
            m_addCommonEntry->exec();
            
            m_addCommonEntryInformation->bindValue(":ide", e.id());
            m_addCommonEntryInformation->bindValue(":title", e.title());
            m_addCommonEntryInformation->bindValue(":id", e.id());
            m_addCommonEntryInformation->exec();
            
            m_addCommonTable->bindValue(":i", c.id());
            m_addCommonTable->bindValue(":e", e.id());
            m_addCommonTable->bindValue(":n", it.key());
            m_addCommonTable->exec();
        }
        
        emit s_updateCommon();
        return true;
    }
    
    return false;
}

QMap<QUuid, Debt> ControllerDB::selectDebt()
{
    QMap<QUuid, Debt> ret;
    if (isConnected()) {
        m_selectDebt->bindValue(":account", m_currentAccount);
        m_selectDebt->bindValue(":profile", m_currentProfile);

        m_selectDebt->exec();

        while (m_selectDebt->next()) {
            if (m_selectDebt->value("removed").toBool())
                continue;

            Debt d;
            d.setId(m_selectDebt->value("id").toUuid());
            d.setNb(m_selectDebt->value("nb").toInt());
            d.setRate(m_selectDebt->value("rate").toDouble());
            d.setName(m_selectDebt->value("name").toString());
            d.setFreq((Account::FrequencyEnum) m_selectDebt->value("freq").toInt());
            ret[d.id()] = d;
        }
    }

    return ret;
}

bool ControllerDB::addDebt(const Debt &d)
{
    bool ret;
    if ((ret = isConnected())) {
        QUuid id = d.id().isNull() ? QUuid::createUuid() : d.id();
        m_addDebt->bindValue(":id", id);
        m_addDebt->bindValue(":freq", (int) d.freq());
        m_addDebt->bindValue(":nb", d.nb());
        m_addDebt->bindValue(":rate", d.rate());
        m_addDebt->bindValue(":name", d.name());
        m_addDebt->bindValue(":account", m_currentAccount);
        m_addDebt->bindValue(":profile", m_currentProfile);

        ret = m_addDebt->exec();

        Entry e;
        e.setAccount(m_currentAccount);
        e.setMetadata("debt", id);
        e.setId(id);
        addEntry(e);

        if (ret)
            emit s_updateDebt();
    }

    return ret;
}

bool ControllerDB::removeDebt(const Debt &d)
{
    Debt dt = d;
    dt.setMetadata("removed", true);

    return updateDebt(dt);
}

bool ControllerDB::updateDebt(const Debt &d)
{
    bool ret;

    if ((ret = isConnected())) {
        m_updateDebt->bindValue(":freq", (int) d.freq());
        m_updateDebt->bindValue(":nb", d.nb());
        m_updateDebt->bindValue(":rate", d.rate());
        m_updateDebt->bindValue(":name", d.name());
        m_updateDebt->bindValue(":removed", d.metaData<bool>("removed"));
        m_updateDebt->bindValue(":account", m_currentAccount);
        m_updateDebt->bindValue(":profile", m_currentProfile);

        ret = m_updateDebt->exec();
        ret &= updateEntry(d.initial());

        qDebug() << ret << m_updateDebt->lastError();
        if (ret)
            emit s_updateDebt();
    }

    return false;
}
