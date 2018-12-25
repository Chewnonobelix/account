#include "budget.h"

Budget::Budget(): QObject(nullptr)
{

}

Budget::Budget(const Budget & b): QObject(nullptr), m_target(b.target()), m_frequency(b.frequency())
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

Budget::BudgetFrequency Budget::frequency() const
{
    return m_frequency;
}

void Budget::setFrequency(BudgetFrequency frequency)
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
    QDate ret = d;
    switch(m_frequency)
    {
    case BudgetFrequency::day:
        ret.addDays(1);
        break;
    case BudgetFrequency::week:
        ret.addDays(8);
        break;
    case BudgetFrequency::month:
        ret.addMonths(1);
        break;
    case BudgetFrequency::quarter:
        ret.addMonths(3);
        break;
    case BudgetFrequency::year:
        ret.addYears(1);
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
    case BudgetFrequency::day:
        ret.addDays(-1);
        break;
    case BudgetFrequency::week:
        ret.addDays(-8);
        break;
    case BudgetFrequency::month:
        ret.addMonths(-1);
        break;
    case BudgetFrequency::quarter:
        ret.addMonths(-3);
        break;
    case BudgetFrequency::year:
        ret.addYears(-1);
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
    Total& t = b.m_current;

    t = t + e;

    return b;
}

Budget& operator- (Budget& b, Entry e)
{
    Entry e2 = e;
    e2.setValue(-1*e.value());

    Total& t = b.m_current;

    t = t + e;

    return b;
}

Budget Budget::clone(QDate d) const
{
    Budget ret(*this);
    ret.setStart(d);

    return ret;
}
