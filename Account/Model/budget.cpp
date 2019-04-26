#include "budget.h"

Budget::Budget(): m_id(-1)
{
}

Budget::Budget(const Budget & b)
{
    //TODO
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

bool Budget::addTarget(QDate, double)
{
    return false;
}

bool Budget::removeTarget(QDate)
{
    //TODO
    return false;
}

bool Budget::updateTarget(QDate, double)
{
    //TODO
    return false;
}

bool Budget::createSub(QDate d)
{
    //TODO
    return false;
}

double Budget::current(QDate)
{
    //TODO
    return 0;
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

Budget& Budget::operator = (const Budget&)
{
    //TODO
    return *this;
}

Budget& Budget::operator <<(Entry)
{
    //TODO
    return *this;
}

Budget& Budget::operator >>(Entry)
{
    //TODO
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
