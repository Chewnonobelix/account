#include "subbudget.h"
#include <QDebug>

SubBudget::SubBudget()
{
    setReference(QDate::currentDate());
    setMetadata("current", 0);
    setTarget(0);
    setBegin(QDate::currentDate());
    setEnd(QDate::currentDate());
}

SubBudget::SubBudget(const SubBudget &sb) : MetaData(sb), m_values(sb.m_values)
{
    update();
}

SubBudget& SubBudget::operator = (const SubBudget& sb)
{
    MetaData::operator=(sb);
    m_values = sb.m_values;
    update();

    return *this;
}

double SubBudget::current() const
{
    return metaData<double>("current");
}

double SubBudget::target() const
{
    return metaData<double>("target");
}

void SubBudget::setTarget(double t)
{
    setMetadata("target", t);
}

void SubBudget::setReference(QDate ref)
{
    setMetadata("reference", ref);
}

QDate SubBudget::reference() const
{
    return metaData<QDate>("reference");
}

QDate SubBudget::begin() const
{
    return metaData<QDate>("begin");
}

void SubBudget::setBegin(QDate d)
{
    setMetadata("begin", d);
}

QDate SubBudget::end() const
{
    return metaData<QDate>("end");
}

void SubBudget::setEnd(QDate d)
{
    setMetadata("end", d);
}

bool SubBudget::addEntry(Entry e)
{
    bool ret = !m_values.contains(e.id());

    if(ret)
    {
        m_values[e.id()] = e.value();
        update();
    }
    return ret;
}

bool SubBudget::removeEntry(Entry e)
{
    bool ret = m_values.contains(e.id());

    if(ret)
    {
        ret = m_values.remove(e.id()) != 0;
        update();
    }

    return ret;
}

bool SubBudget::updateEntry(Entry e)
{
    bool ret = m_values.contains(e.id());

    if(ret)
    {
        m_values[e.id()] = e.value();
        update();
    }
    return ret;
}

void SubBudget::update()
{
    double temp = 0;
    for(auto it: m_values)
        temp += it;

    setMetadata("current", temp);
}

bool SubBudget::in(QDate d) const
{
    return (begin() <= d) && (d <= end());
}

double SubBudget::percent() const
{
     int temp = current() / target() * 10000;
     return temp / 100.0;
}
