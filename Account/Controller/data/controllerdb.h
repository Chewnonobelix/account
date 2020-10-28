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
    bool addEntry(Entry&)  override;
    QMap<QUuid, Entry> selectEntry() override;
    bool removeEntry(Entry&) override;

    QStringList selectAccount(QString) override;
    bool removeAccount(QString) override;

    bool updateEntry(Entry &) override;

	bool addCategory(Category &) override;
	bool removeCategory(QString) override;
    QMap<Account::TypeEnum, QMap<QUuid, Category>> selectCategory() override;
    virtual bool updateCategory(Category &) override { return false; }

    virtual bool addBudget(Budget&) override;
    virtual bool removeBudget(Budget&) override;
    virtual QMap<QUuid, Budget> selectBudgets() override;
    virtual bool updateBudget(Budget &) override;
    
    virtual bool addFrequency(Frequency&) override;
    virtual bool removeFrequency(Frequency&) override;
    virtual bool updateFrequency(Frequency&) override;
    virtual QMap<QUuid, Frequency> selectFrequency() override;
    
    virtual QMap<QUuid, CommonExpanse> selectCommon() override;
    virtual bool addCommon(CommonExpanse&) override;
    virtual bool removeCommon(CommonExpanse&) override;
    virtual bool updateCommon(CommonExpanse&) override;
    
    virtual QStringList selectProfile() override;
    virtual bool addProfile(QString, QString) override;
    virtual bool deleteProfile(QString) override;

    virtual QMap<QUuid, Debt> selectDebt() override;
    virtual bool addDebt(Debt &) override;
    virtual bool removeDebt(Debt &) override;
    virtual bool updateDebt(Debt &) override;

    bool init() override;
    QSharedPointer<InterfaceDataSave> clone() const override;
};

Q_DECLARE_METATYPE(ControllerDB)

#endif // CONTROLLERDB_H
