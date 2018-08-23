#include "total.h"

Total::Total()
{

}

Total::Total(const Total& t): m_date(t.date()), m_value(t.value())
{}

Total::~Total() {}

Total& Total::operator = (const Total& t)
{
    setDate(t.date());
    setValue(t.value());

    return *this;
}

Total operator+(const Entry& e1, const Entry& e2)
{
    Total ret;

    ret.setValue(e1.value() + e2.value());
    ret.setDate(Total::maxDate(e1.date(), e2.date()));

    return ret;
}

Total& operator + (Total& t, const Entry& e)
{
       t.setValue(t.value() + e.value());
       t.setDate(Total::maxDate(t.date(), e.date()));

       return t;
}

Total& operator + (const Entry& e, Total& t)
{
    return (t + e);
}


QDate Total::date() const
{
    return m_date;
}

void Total::setDate(QDate date)
{
    m_date = date;
}

double Total::value() const
{
    return m_value;
}

void Total::setValue(double value)
{
    m_value = value;
}

QDate Total::maxDate(const QDate & d1, const QDate & d2)
{
    if(d1 < d2)
        return d2;
    else
        return d1;
}
