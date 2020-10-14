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
    virtual bool addEntry(Entry&) = 0;
    virtual QMultiMap<QDate, Entry> selectEntry() = 0;
    virtual bool removeEntry(Entry&) = 0;

    virtual QStringList selectAccount(QString = "") = 0;
    virtual bool removeAccount(QString) = 0;

    virtual bool updateEntry(Entry &) = 0;

	virtual bool addCategory(Category &) = 0;
	virtual bool removeCategory(QString) = 0;
    virtual QMap<Account::TypeEnum, QMap<QUuid, Category>> selectCategory() = 0;
    virtual bool updateCategory(Category &) = 0;

    virtual bool addBudget(Budget &) = 0;
    virtual bool removeBudget(Budget&) = 0;
    virtual QList<Budget> selectBudgets() = 0;
    virtual bool updateBudget(Budget&) = 0;
    
    
    virtual bool addFrequency(Frequency&) = 0;
    virtual bool removeFrequency(Frequency&) = 0;
    virtual bool updateFrequency(Frequency&) = 0;
    virtual QList<Frequency> selectFrequency() = 0;
    
    virtual QMap<QUuid, CommonExpanse> selectCommon() = 0;
    virtual bool addCommon(CommonExpanse&) = 0;
    virtual bool removeCommon(CommonExpanse&) = 0;
    virtual bool updateCommon(CommonExpanse &) = 0;

    virtual QMap<QUuid, Debt> selectDebt() = 0;
    virtual bool addDebt(Debt &) = 0;
    virtual bool removeDebt(Debt &) = 0;
    virtual bool updateDebt(Debt &) = 0;

    virtual QStringList selectProfile() = 0;
    virtual bool addProfile(QString, QString) = 0;
    virtual bool deleteProfile(QString) = 0;

    virtual QMap<QUuid, SynchronizationProfile> selectSyncProfile();
    virtual bool removeSyncProfile(SynchronizationProfile&);
    virtual QUuid addSyncProfile(SynchronizationProfile &);
    virtual bool updateSyncProfile(SynchronizationProfile&);
        
    virtual bool init() = 0;
    virtual QSharedPointer<InterfaceDataSave> clone() const = 0;
};

#endif // INTERFACEDATASAVE_H
