#ifndef CONTROLLERDB_H
#define CONTROLLERDB_H

#include "../../Model/entry.h"
#include "interfacedatasave.h"
#include <QSharedPointer>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlResult>
#include <QVariant>

class ControllerDB: public InterfaceDataSave
{
    typedef QSharedPointer<QSqlQuery> SqlQuery;
    Q_OBJECT
    
private:
    QSqlDatabase m_db;
    
    SqlQuery m_selectProfiles;
    SqlQuery m_removeProfile;
    
    SqlQuery m_accounts;
    SqlQuery m_removeAccount;
    
    SqlQuery m_addEntry;
    SqlQuery m_removeEntry;
    SqlQuery m_selectEntry;
    SqlQuery m_updateEntry;
    SqlQuery m_insertMetadata;
    SqlQuery m_updateMetadata;
    SqlQuery m_selectMetadata;
    SqlQuery m_removeMetadata;
        
    SqlQuery m_addCategory;
    SqlQuery m_removeCategory;
    SqlQuery m_selectCategory;
    
    SqlQuery m_addBudget;
    SqlQuery m_removeBudget;
    SqlQuery m_selectBudget;
    SqlQuery m_updateBudget;
    SqlQuery m_selectSubBudget;
    SqlQuery m_updateSubbudget;
    SqlQuery m_removeSubbudget;
    SqlQuery m_addSubbudget;
    
    SqlQuery m_addFrequency;
    SqlQuery m_removeFrequency;
    SqlQuery m_selectFrequency;
    SqlQuery m_updateFrequency;
    SqlQuery m_addFrequencyReference;
    SqlQuery m_removeFrequencyReference;
    SqlQuery m_selectFrequencyReference;
    SqlQuery m_updateFrequencyReference;
    
    SqlQuery m_addCommon;
    SqlQuery m_removeCommon;
    SqlQuery m_selectCommon;
    SqlQuery m_updateCommon;
    SqlQuery m_addCommonEntry;
    SqlQuery m_addCommonTable;
    SqlQuery m_selectCommonEntry;
    SqlQuery m_selectCommonTable;
    SqlQuery m_addCommonEntryInformation;

    SqlQuery m_addDebt;
    SqlQuery m_updateDebt;
    SqlQuery m_selectDebt;
    SqlQuery m_removeDebt;

    QString m_currentAccount;

    void prepareEntry();
    void prepareAccount();
    void prepareCategory();
    void prepareBudget();
    void prepareFrequency();
    void prepareCommon();
    void prepareProfile();
    void prepareDebt();

public:
    ControllerDB(bool = false);
    ControllerDB(const ControllerDB&d);
    ~ControllerDB();
    
    bool isConnected() const;
public slots:
    bool addEntry(const Entry&)  override;
    QMultiMap<QDate, Entry> selectEntry() override;
    bool removeEntry(const Entry&) override;
    
    QStringList selectAccount() override;
    bool removeAccount(QString) override;

    bool updateEntry(const Entry &) override;
    
    bool addCategory(QString, QString) override;
    bool removeCategory(QString) override;
    QMultiMap<QString, QString> selectCategory() override;
    
    virtual bool addBudget(const Budget&) override;
    virtual bool removeBudget(const Budget&) override;
    virtual QList<Budget> selectBudgets() override;
    virtual bool updateBudget(const Budget &) override;
    
    virtual bool addFrequency(const Frequency&) override;
    virtual bool removeFrequency(const Frequency&) override;
    virtual bool updateFrequency(const Frequency&) override;
    virtual QList<Frequency> selectFrequency() override;
    
    virtual QMap<QUuid, CommonExpanse> selectCommon() override;
    virtual bool addCommon(const CommonExpanse&) override;
    virtual bool removeCommon(const CommonExpanse&) override;
    virtual bool updateCommon(const CommonExpanse&) override;
    
    virtual QStringList selectProfile() override;
    virtual bool addProfile(QString, QString) override;
    virtual bool deleteProfile(QString) override;

    virtual QMap<QUuid, Debt> selectDebt();
    virtual bool addDebt(const Debt &);
    virtual bool removeDebt(const Debt &);
    virtual bool updateDebt(const Debt &);

    bool init() override;
};

Q_DECLARE_METATYPE(ControllerDB)

#endif // CONTROLLERDB_H
