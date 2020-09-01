#include "synchronizationprofile.h"

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

QString SynchronizationProfile::idString() const
{
    return id().toString();
}

QString SynchronizationProfile::toString() const
{
    return QString();
}
