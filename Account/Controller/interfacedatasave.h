#ifndef INTERFACEDATASAVE_H
#define INTERFACEDATASAVE_H

#include <QDomDocument>
#include <QString>
#include <QList>
#include <QFile>
#include "Model/entry.h"
#include "Model/budget.h"
#include "Model/frequency.h"
#include "Model/accountglobal.h"
#include "Model/commonexpanse.h"
#include "Model/synchronizationprofile.h"

class InterfaceDataSave: public QObject
{
    Q_OBJECT
private:
    QDomDocument m_syncs;
    
protected:
    bool backup = false;
    QString m_currentProfile = "Default";
    QString m_path = "data";

public:
    InterfaceDataSave();
    inline InterfaceDataSave(const InterfaceDataSave&): QObject(nullptr) {}
    virtual ~InterfaceDataSave();
    inline void setBackup(bool back) { backup = back;}
    inline void setPath(QString path) {m_path = path;}

signals:
    void s_updateEntry(QUuid = QUuid());
    void s_updateCategory();
    void s_updateBudget();
    void s_updateFrequency();
    void s_updateCommon();
    void s_updateProfile();
    void s_updateAccount();
    
public slots:
    void exec();
    virtual bool addEntry(const Entry&) = 0;
    virtual QMultiMap<QDate, Entry> selectEntry(QString) = 0;
    virtual bool removeEntry(const Entry&) = 0;
    
    virtual QStringList selectAccount() = 0;
    virtual bool removeAccount(QString) = 0;
    virtual void setCurrentAccount(QString) = 0;

    virtual bool updateInfo(const Entry&) = 0;
    virtual bool updateEntry(const Entry&) = 0;
    
    virtual bool addCategory(QString, QString) = 0;
    virtual bool removeCategory(QString) = 0;
    virtual QMultiMap<QString, QString> selectCategory() = 0;
    
    virtual bool addBudget(const Budget&) = 0;
    virtual bool removeBudget(const Budget&) = 0;
    virtual QList<Budget> selectBudgets() = 0;
    virtual bool updateBudget(const Budget&) = 0;
    
    
    virtual bool addFrequency(const Frequency&) = 0;
    virtual bool removeFrequency(const Frequency&) = 0;
    virtual bool updateFrequency(const Frequency&) = 0;
    virtual QList<Frequency> selectFrequency() = 0;
    
    virtual QMap<int, CommonExpanse> selectCommon() = 0;
    virtual bool addCommon(const CommonExpanse&) = 0;
    virtual bool removeCommon(const CommonExpanse&) = 0;
    virtual bool updateCommon(const CommonExpanse&) = 0;
    
    virtual QStringList selectProfile() = 0;
    virtual void setProfile(QString) = 0;
    virtual bool addProfile(QString, QString) = 0;
    virtual QString currentProfile() = 0;
    virtual bool deleteProfile(QString) = 0;

    virtual QList<SynchronizationProfile> selectSyncProfile();
    virtual bool removeSyncProfile(const SynchronizationProfile&);
    virtual bool addSyncProfile(const SynchronizationProfile&);
    virtual bool updateSyncProfile(const SynchronizationProfile&);
        
    virtual bool init() = 0;
};

#endif // INTERFACEDATASAVE_H
