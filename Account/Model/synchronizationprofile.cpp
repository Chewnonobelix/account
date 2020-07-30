#include "synchronizationprofile.h"

QString SynchronizationProfile::hostName() const
{
    return m_serverName;
}

void SynchronizationProfile::setHostName(QString s)
{
    m_serverName = s;
}

QString SynchronizationProfile::deviceName() const
{
    return m_deviceName;
}

void SynchronizationProfile::setDeviceName(QString d)
{
    m_deviceName =d;
}

QDate SynchronizationProfile::begin() const
{
    return m_begin;
}

void SynchronizationProfile::setbegin(QDate d)
{
    m_begin = d;
}

QDate SynchronizationProfile::end() const
{
    return m_end;
}

void SynchronizationProfile::setEnd(QDate e)
{
    m_end =e;
}

QDateTime SynchronizationProfile::lastSync() const
{
    return m_lastSync;
}

void SynchronizationProfile::setLastSync(QDateTime ls)
{
    m_lastSync = ls;
}

