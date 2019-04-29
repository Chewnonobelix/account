#include "subbudget.h"

SubBudget::SubBudget()
{

}

SubBudget::SubBudget(const SubBudget& sb): m_target(sb.target()), m_begin(sb.begin()),
    m_end(sb.end()), m_values(sb.m_values)
{
    update();
}

SubBudget& SubBudget::operator = (const SubBudget& sb)
{
    m_target = sb.target();
    m_begin = sb.begin();
    m_end = sb.end();
    m_values = sb.m_values;
    update();

    return *this;
}

double SubBudget::current() const
{
    return m_current;
}

double SubBudget::target() const
{
    return m_target;
}

void SubBudget::setTarget(double t)
{
    m_target = t;
}

QDate SubBudget::begin() const
{
    return m_begin;
}

void SubBudget::setBegin(QDate d)
{
    m_begin = d;
}

QDate SubBudget::end() const
{
    return m_end;
}

void SubBudget::setEnd(QDate d)
{
    m_end = d;
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

    m_current = temp;
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
