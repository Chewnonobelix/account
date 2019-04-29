#include "budget.h"

Budget::Budget(): m_id(-1)
{
}

Budget::Budget(const Budget & b)
{
    m_id = b.id();
    m_category = b.category();
    m_reference = b.reference();
    m_frequency = b.frequency();
    m_targets = b.m_targets;
    m_subs = b.m_subs;
}

int Budget::id() const
{
    return m_id;
}

void Budget::setId(int i)
{
    m_id = i;
}

bool Budget::addEntry(Entry e)
{
    bool ret = false;

    for(auto it: m_subs)
        if(it.begin() <= e.date() && e.date() <= it.end())
            ret = it.addEntry(e);

    return ret;
}

bool Budget::removeEntry(Entry e)
{
    bool ret = false;

    for(auto it: m_subs)
        if(it.begin() <= e.date() && e.date() <= it.end())
            ret = it.removeEntry(e);

    return ret;
}

bool Budget::updateEntry(Entry e)
{
    bool ret = false;

    for(auto it: m_subs)
        if(it.begin() <= e.date() && e.date() <= it.end())
            ret = it.updateEntry(e);

    return ret;
}

bool Budget::addTarget(QDate d, double t)
{
    bool ret = !m_targets.contains(d);

    if(ret)
        m_targets[d] = t;

    return ret;
}

bool Budget::removeTarget(QDate d)
{
    return m_targets.remove(d) != 0;
}

bool Budget::updateTarget(QDate d, double t)
{
    bool ret = m_targets.contains(d);

    if(ret)
        m_targets[d] = t;
    return ret;
}

bool Budget::createSub(QDate d)
{
    QDate start = reference();
    QDate end = next(start).addDays(-1);
    bool ret = false;
    SubBudget sub;
    sub.setBegin(start);
    sub.setEnd(end);

    while(!sub.in(d))
    {
        if(d < start)
        {
            start = previous(start);
            end = previous(end);
        }
        else
        {
            start = next(start);
            end = next(end);
        }

        sub.setBegin(start);
        sub.setEnd(end);
    }

    auto l = m_targets.keys();

    double tar = -1;
    if(l.size() == 1 || d < l.first())
        tar = m_targets.first();
    else
    {
        for(auto it = l.begin(); it != l.end(); it++)
        {
            if(d < *it)
            {
                tar = m_targets[*(it-1)];
            }
        }
    }

    sub.setTarget(tar);
    m_subs[sub.begin()] = sub;
    return ret;
}

double Budget::current(QDate d)
{
    double ret = 0;
    for(auto it: m_subs)
        if(it.in(d))
            ret = it.current();
    return ret;
}

Account::FrequencyEnum Budget::frequency() const
{
    return m_frequency;
}

void Budget::setFrequency(Account::FrequencyEnum f)
{
    m_frequency = f;
}

QString Budget::category() const
{
    return m_category;
}

void Budget::setCategory(QString c)
{
    m_category = c;
}

Budget& Budget::operator = (const Budget& b)
{
    m_id = b.id();
    m_category = b.category();
    m_reference = b.reference();
    m_frequency = b.frequency();
    m_targets = b.m_targets;
    m_subs = b.m_subs;

    return *this;
}

Budget& Budget::operator <<(Entry e)
{
    addEntry(e);
    return *this;
}

Budget& Budget::operator >>(Entry e)
{
    removeEntry(e);
    return *this;
}

QDate Budget::next(QDate d) const
{
    switch(frequency())
    {
    case Account::FrequencyEnum::day:
        return d.addDays(1);
    case Account::FrequencyEnum::week:
        return d.addDays(8);
    case Account::FrequencyEnum::month:
        return d.addMonths(1);
    case Account::FrequencyEnum::quarter:
        return d.addMonths(4);
    case Account::FrequencyEnum::year:
        return d.addYears(1);
    default:
        return d;
    }
}

QDate Budget::previous(QDate d) const
{
    switch(frequency())
    {
    case Account::FrequencyEnum::day:
        return d.addDays(-1);
    case Account::FrequencyEnum::week:
        return d.addDays(-8);
    case Account::FrequencyEnum::month:
        return d.addMonths(-1);
    case Account::FrequencyEnum::quarter:
        return d.addMonths(-4);
    case Account::FrequencyEnum::year:
        return d.addYears(-1);
    default:
        return d;
    }
}

QDate Budget::reference() const
{
    return m_reference;
}

void Budget::setReference(QDate d)
{
    m_reference = d;
}

QMap<QDate, double> Budget::targets() const
{
    return m_targets;
}

QMap<QDate, SubBudget> Budget::subs() const
{
    return m_subs;
}
