#ifndef SYNCHRONIZATIONPROFILE_H
#define SYNCHRONIZATIONPROFILE_H

#include "account_global.h"
#include "entry.h"
#include "metadata.h"
#include <QDate>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QUuid>

class ACCOUNT_EXPORT SynchronizationProfile : public MetaData
{
    Q_GADGET
    
    Q_PROPERTY(QString hostName READ hostName)
    Q_PROPERTY(QString deviceName READ deviceName)
    Q_PROPERTY(QDate begin READ begin)
    Q_PROPERTY(QDate end READ end)
    Q_PROPERTY(QDateTime lastSync READ lastSync)
    Q_PROPERTY(QUuid id READ id)

private:
public:
    SynchronizationProfile() = default;
    SynchronizationProfile(const SynchronizationProfile&) = default;
    ~SynchronizationProfile() = default;
    SynchronizationProfile &operator=(const SynchronizationProfile &);

    QUuid id() const;
    void setId(QUuid);
    QString hostName() const;
    void setHostName(QString);
    QString deviceName() const;
    void setDeviceName(QString);
    QDate begin() const;
    void setBegin(QDate);
    QDate end() const;
    void setEnd(QDate);
    QDateTime lastSync() const;
    void setLastSync(QDateTime);
    QMap<QUuid, Entry> document() const;
    void setDocument(QJsonDocument);

    QString toString() const;
};

Q_DECLARE_METATYPE(SynchronizationProfile)
#endif // SYNCHRONIZATIONPROFILE_H
