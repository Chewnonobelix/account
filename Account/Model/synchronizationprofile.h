#ifndef SYNCHRONIZATIONPROFILE_H
#define SYNCHRONIZATIONPROFILE_H

#include <QDate>
#include <QObject>
#include "account_global.h"

class ACCOUNT_EXPORT SynchronizationProfile
{
    Q_GADGET
    
    Q_PROPERTY(QString hostName READ hostName)
    Q_PROPERTY(QString deviceName READ deviceName)
    Q_PROPERTY(QDate begin READ begin)
    Q_PROPERTY(QDate end READ end)
    Q_PROPERTY(QDateTime lastSync READ lastSync)
    
private:
    QString m_serverName = "";
    QString m_deviceName = "";
    QDate m_begin = QDate::currentDate();
    QDate m_end = QDate::currentDate();
    QDateTime m_lastSync = QDateTime::currentDateTime();
    
public:
    SynchronizationProfile() = default;
    SynchronizationProfile(const SynchronizationProfile&) = default;
    ~SynchronizationProfile() = default;
    SynchronizationProfile& operator= (const SynchronizationProfile&) = default;

    QString hostName() const;
    void setHostName(QString);
    QString deviceName() const;
    void setDeviceName(QString);
    QDate begin() const;
    void setbegin(QDate);
    QDate end() const;
    void setEnd(QDate);
    QDateTime lastSync() const;
    void setLastSync(QDateTime);
    
};

#endif // SYNCHRONIZATIONPROFILE_H
