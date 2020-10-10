#include "synchronizationprofile.h"

SynchronizationProfile &SynchronizationProfile::operator=(const SynchronizationProfile &s)
{
    MetaData::operator=(s);
    return *this;
}

QString SynchronizationProfile::hostName() const
{
    return metaData<QString>("hostName");
}

void SynchronizationProfile::setHostName(QString s)
{
    setMetadata("hostName", s);
}

QString SynchronizationProfile::deviceName() const
{
    return metaData<QString>("deviceName");
}

void SynchronizationProfile::setDeviceName(QString d)
{
    setMetadata("deviceName", d);
}

QDate SynchronizationProfile::begin() const
{
    return metaData<QDate>("begin");
}

void SynchronizationProfile::setBegin(QDate d)
{
    setMetadata("begin", d);
}

QDate SynchronizationProfile::end() const
{
    return metaData<QDate>("end");
}

void SynchronizationProfile::setEnd(QDate e)
{
    setMetadata("end", e);
}

QDateTime SynchronizationProfile::lastSync() const
{
    return metaData<QDateTime>("lastSync");
}

void SynchronizationProfile::setLastSync(QDateTime ls)
{
    setMetadata("lastSync", ls);
}

QUuid SynchronizationProfile::id() const
{
    return metaData<QUuid>("id");
}

void SynchronizationProfile::setId(QUuid id)
{
    setMetadata("id", id);
}

QString SynchronizationProfile::toString() const
{
    return QString();
}

QJsonDocument SynchronizationProfile::document() const
{
    return metaData<QJsonDocument>("document");
}

void SynchronizationProfile::setDocument(QJsonDocument json)
{
    setMetadata("document", json);
}
