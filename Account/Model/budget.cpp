#include "budget.h"

Budget::Budget()
{

}

Budget::Budget(const Budget & b): m_target(b.target()), m_frequency(b.frequency())
{

}

double Budget::target() const
{
    return m_target;
}

void Budget::setTarget(double target)
{
    m_target = target;
}

Account::FrequencyEnum  Budget::frequency() const
{
    return m_frequency;
}

void Budget::setFrequency(Account::FrequencyEnum  frequency)
{
    m_frequency = frequency;
}

QDate Budget::start() const
{
    return m_start;
}

void Budget::setStart(QDate start)
{
    m_start = start;
}

QDate Budget::next(QDate d) const
{
    QDate ret;
    switch(m_frequency)
    {
    case Account::FrequencyEnum::day:
        ret = d.addDays(1);
        break;
    case Account::FrequencyEnum ::week:
        ret = d.addDays(8);
        break;
    case Account::FrequencyEnum ::month:
        ret = d.addMonths(1);
        break;
    case Account::FrequencyEnum ::quarter:
        ret = d.addMonths(3);
        break;
    case Account::FrequencyEnum ::year:
        ret = d.addYears(1);
        break;

    default:
        break;
    }

    return ret;
}

QDate Budget::prev(QDate d) const
{
    QDate ret = d;
    switch(m_frequency)
    {
    case Account::FrequencyEnum ::day:
        ret = d.addDays(-1);
        break;
    case Account::FrequencyEnum ::week:
        ret = d.addDays(-8);
        break;
    case Account::FrequencyEnum ::month:
        ret = d.addMonths(-1);
        break;
    case Account::FrequencyEnum ::quarter:
        ret = d.addMonths(-3);
        break;
    case Account::FrequencyEnum ::year:
        ret = d.addYears(-1);
        break;

    default:
        break;
    }

    return ret;
}

bool Budget::in(QDate d) const
{
    QDate n = next(start());

   return (d >= start()) && (d < n);
}

Budget& operator+ (Budget& b, Entry e)
{
   if(!b.m_related.contains(e.id()))
   {
       Total& t = b.m_current;

       t = t + e;
       b.m_related<<e.id();
   }

    return b;
}

Budget& operator- (Budget& b, Entry e)
{
    if(b.m_related.remove(e.id()))
    {
        Entry e2 = e;
        e2.setValue(-1*e.value());

        Total& t = b.m_current;

        t = t + e;
    }

    return b;
}

Budget Budget::clone(QDate d) const
{
    Budget ret(*this);
    ret.setStart(d);

    return ret;
}
