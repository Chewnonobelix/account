#ifndef INTERFACEDATASAVE_H
#define INTERFACEDATASAVE_H

#include "Model/accountglobal.h"
#include "Model/budget.h"
#include "Model/category.h"
#include "Model/commonexpanse.h"
#include "Model/debt.h"
#include "Model/entry.h"
#include "Model/frequency.h"
#include "Model/synchronizationprofile.h"
#include "designpattern.h"
#include <QDomDocument>
#include <QFile>
#include <QList>
#include <QString>

class InterfaceDataSave: public QObject
{
    Q_OBJECT
private:
    QDomDocument m_syncs;
    
protected:
    bool backup = false;
    QString m_currentProfile = "Default";
    QString m_currentAccount;
    QString m_path = "data";

public:
    InterfaceDataSave();
    InterfaceDataSave(const InterfaceDataSave &);
    virtual ~InterfaceDataSave();
    inline void setBackup(bool back) { backup = back;}
    inline void setPath(QString path) { m_path = path; }

    void setProfile(QString);
    QString currentProfile() const;
    void setCurrentAccount(QString);
    QString currentAccount() const;

signals:
    void s_updateEntry(QUuid = QUuid());
    void s_updateCategory();
    void s_updateBudget();
    void s_updateFrequency();
    void s_updateCommon();
    void s_updateProfile();
    void s_updateAccount();
    void s_updateDebt();
    void s_updateSync();

public slots:
    void exec();
    virtual bool addEntry(const Entry&) = 0;
    virtual QMultiMap<QDate, Entry> selectEntry() = 0;
    virtual bool removeEntry(const Entry&) = 0;

    virtual QStringList selectAccount(QString = "") = 0;
    virtual bool removeAccount(QString) = 0;

    virtual bool updateEntry(const Entry &) = 0;

    virtual bool addCategory(const Category &) = 0;
    virtual bool removeCategory(QString) = 0;
    virtual QMultiMap<Account::TypeEnum, Category> selectCategory() = 0;
    virtual bool updateCategory(const Category &) = 0;

    virtual bool addBudget(const Budget &) = 0;
    virtual bool removeBudget(const Budget&) = 0;
    virtual QList<Budget> selectBudgets() = 0;
    virtual bool updateBudget(const Budget&) = 0;
    
    
    virtual bool addFrequency(const Frequency&) = 0;
    virtual bool removeFrequency(const Frequency&) = 0;
    virtual bool updateFrequency(const Frequency&) = 0;
    virtual QList<Frequency> selectFrequency() = 0;
    
    virtual QMap<QUuid, CommonExpanse> selectCommon() = 0;
    virtual bool addCommon(const CommonExpanse&) = 0;
    virtual bool removeCommon(const CommonExpanse&) = 0;
    virtual bool updateCommon(const CommonExpanse &) = 0;

    virtual QMap<QUuid, Debt> selectDebt() = 0;
    virtual bool addDebt(const Debt &) = 0;
    virtual bool removeDebt(const Debt &) = 0;
    virtual bool updateDebt(const Debt &) = 0;

    virtual QStringList selectProfile() = 0;
    virtual bool addProfile(QString, QString) = 0;
    virtual bool deleteProfile(QString) = 0;

    virtual QMap<QUuid, SynchronizationProfile> selectSyncProfile();
    virtual bool removeSyncProfile(const SynchronizationProfile&);
    virtual QUuid addSyncProfile(const SynchronizationProfile &);
    virtual bool updateSyncProfile(const SynchronizationProfile&);
        
    virtual bool init() = 0;
    virtual QSharedPointer<InterfaceDataSave> clone() const = 0;
};

#endif // INTERFACEDATASAVE_H
