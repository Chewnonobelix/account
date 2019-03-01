#include "information.h"

Information::Information(): m_id(-1), m_estimated(false)
{

}

Information::Information(const Information & i):
    m_id(i.id()), m_idEntry(i.idEntry()), m_title(i.title()), m_estimated(i.estimated()),
    m_category(i.category())
{}

Information::~Information() {}

Information& Information::operator =(const Information& i)
{
    setId(i.id());
    setIdEntry(i.idEntry());
    setTitle(i.title());
    setEstimated(i.estimated());
    setCategory(i.category());

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

int Information::id() const
{
    return m_id;
}

void Information::setId(int id)
{
    m_id = id;
}

int Information::idEntry() const
{
    return m_idEntry;
}

void Information::setIdEntry(int idEntry)
{
    m_idEntry = idEntry;
}

QString Information::title() const
{
    return m_title;
}

void Information::setTitle(QString title)
{
    m_title = title;
}

bool Information::estimated() const
{
    return m_estimated;
}

void Information::setEstimated(bool estimated)
{
    m_estimated = estimated;
}

QString Information::category() const
{
    return m_category;
}

void Information::setCategory(QString category)
{
    m_category = category;
}

