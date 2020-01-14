#ifndef CONTROLLERDB_H
#define CONTROLLERDB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSharedPointer>
#include <QSqlError>
#include "../Model/entry.h"
#include "interfacedatasave.h"

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
    
    SqlQuery m_updateInfo;
    SqlQuery m_addInformation;
    SqlQuery m_removeInformation;
    SqlQuery m_selectInformation;

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
    
    QString m_currentProfile;
    QString m_currentAccount;

    void prepareEntry();
    void prepareInformation();
    void prepareAccount();
    void prepareCategory();
    void prepareBudget();
    void prepareFrequency();
    void prepareCommon();
    void prepareProfile();

public:
    ControllerDB();
    ControllerDB(const ControllerDB&d);
    ~ControllerDB();

    bool isConnected() const;
public slots:
    bool addEntry(QSharedPointer<Entry>) {return false;}
    bool addEntry(const Entry&);
    QMultiMap<QDate, Entry> selectEntry(QString);
    bool removeEntry(const Entry&);

    QStringList selectAccount();
    bool removeAccount(QString);

    bool updateInfo(const Entry&);
    bool updateEntry(const Entry &);

    bool addCategory(QString, QString);
    bool removeCategory(QString);
    QMultiMap<QString, QString> selectCategory();

    virtual bool addBudget(const Budget&);
    virtual bool removeBudget(const Budget&);
    virtual QList<Budget> selectBudgets();
    virtual bool updateBudget(const Budget &);

    virtual bool addFrequency(const Frequency&);
    virtual bool removeFrequency(const Frequency&);
    virtual bool updateFrequency(const Frequency&);
    virtual QList<Frequency> selectFrequency();
    
    virtual QMap<int, CommonExpanse> selectCommon();
    virtual bool addCommon(const CommonExpanse&);
    virtual bool removeCommon(const CommonExpanse&);
    virtual bool updateCommon(const CommonExpanse&);
    
    virtual QStringList selectProfile();
    virtual void setProfile(QString);
    virtual bool addProfile(QString, QString);
    virtual QString currentProfile();
    virtual bool deleteProfile(QString);

    bool init();
};

Q_DECLARE_METATYPE(ControllerDB)

#endif // CONTROLLERDB_H
