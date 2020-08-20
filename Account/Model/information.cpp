#include "information.h"

Information::Information()
{
    setId(QUuid());
    setIdEntry(QUuid());
    setTitle(QString());
    setEstimated(false);
    setCategory(QString());
}

Information& Information::operator =(const Information& i)
{
    MetaData::operator=(i);

    return *this;
}

bool operator == (const Information& i1, const Information& i2)
{
    return i1.id() == i2.id();
}

bool operator <(const Information& i1, const Information& i2)
{
    return i1.title() < i2.title();
}

QUuid Information::id() const
{
    return metaData<QUuid>("id");
}

void Information::setId(QUuid id)
{
    setMetadata("id", id);
}

QUuid Information::idEntry() const
{
    return metaData<QUuid>("ide");
}

void Information::setIdEntry(QUuid idEntry)
{
    setMetadata("ide", idEntry);
}

QString Information::title() const
{
    return metaData<QString>("title");
}

void Information::setTitle(QString title)
{
    setMetadata("title", title);
}

bool Information::estimated() const
{
    return metaData<bool>("estimated");
}

void Information::setEstimated(bool estimated)
{
    setMetadata("estimated", estimated);
}

QString Information::category() const
{
    return metaData<QString>("category");
}

void Information::setCategory(QString category)
{
    setMetadata("category", category);
}

