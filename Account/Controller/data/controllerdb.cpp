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

QSharedPointer<InterfaceDataSave> ControllerDB::clone() const
{
    return DesignPattern::factory<ControllerDB>(*this);
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

QStringList ControllerDB::selectAccount(QString)
{
    QStringList res;
    
    return res;
}

bool ControllerDB::removeAccount(QString name)
{
    bool ret = false;

    return ret;
}

QStringList ControllerDB::selectProfile() 
{
    QStringList ret;    
    return ret;
}

bool ControllerDB::addProfile(QString name, QString) 
{
    bool ret = false;
    return ret;
}

bool ControllerDB::deleteProfile(QString name) 
{    
    return false;
}

bool ControllerDB::addEntry(Entry & e)
{        
    bool ret = false;
    
    return ret;
}

QMultiMap<QDate, Entry> ControllerDB::selectEntry()
{
    QMultiMap<QDate, Entry> res;
        
    return res;
}

bool ControllerDB::updateEntry(Entry & e)
{
    bool ret = false;
    return ret;
}

bool ControllerDB::removeEntry(Entry & e)
{
    bool ret = false;
    
    return ret;
}

bool ControllerDB::addCategory(Category &n)
{    
    return false;
}

bool ControllerDB::removeCategory(QString name)
{    
    return false;
}

QMap<Account::TypeEnum, QMap<QUuid, Category>> ControllerDB::selectCategory()
{
    QMap<Account::TypeEnum, QMap<QUuid, Category>> ret;

    return ret;
}

bool ControllerDB::addBudget(Budget& b)
{    
    return false;
}

bool ControllerDB::removeBudget(Budget & b)
{    
    return false;
}

QList<Budget> ControllerDB::selectBudgets()
{
    QList<Budget> ret;
    
    return ret;
}

bool ControllerDB::updateBudget(Budget & b)
{    
    return false;
}

bool ControllerDB::addFrequency(Frequency & f)
{
    return false;
}

bool ControllerDB::removeFrequency(Frequency& f)
{    
    return false;
}

bool ControllerDB::updateFrequency(Frequency& f)
{
    return false;
}

QList<Frequency> ControllerDB::selectFrequency()
{
    QList<Frequency> ret;
    
    return ret;
}

QMap<QUuid, CommonExpanse> ControllerDB::selectCommon()
{
    QMap<QUuid, CommonExpanse> ret;
        
    return  ret; 
}

bool ControllerDB::addCommon(CommonExpanse& c)
{
    return false;
}

bool ControllerDB::removeCommon(CommonExpanse& c)
{    
    return false;
}

bool ControllerDB::updateCommon(CommonExpanse& c)
{
    return false;
}

QMap<QUuid, Debt> ControllerDB::selectDebt()
{
    QMap<QUuid, Debt> ret;

    return ret;
}

bool ControllerDB::addDebt(Debt &d)
{
    bool ret = false;

    return ret;
}

bool ControllerDB::removeDebt(Debt &d)
{
    return false;
}

bool ControllerDB::updateDebt(Debt &d)
{
    return false;
}
