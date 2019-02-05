#include "total.h"


#include <QDebug>

Total::Total(): m_value(0)
{

}

Total::Total(const Total& t): m_date(t.date()), m_value(t.value())
{
}

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

    QMetaEnum qme = QMetaEnum::fromType<Categories::Type>();

    ret.setValue((qme.keyToValue(e1.type().toLower().toLatin1())*e1.value()) + (qme.keyToValue(e1.type().toLower().toLatin1())*e2.value()));
    ret.setDate(Total::maxDate(e1.date(), e2.date()));

    ret.setValue(round (ret.value() * 100.0) / 100.0);

    return ret;
}

Total operator + (const Total& t, const Total& t2)
{
    Total ret;
    ret.setValue(t.value() + t2.value());
    ret.setDate(Total::maxDate(t.date(), t2.date()));
    ret.setValue(round (ret.value() * 100.0) / 100.0);
    return ret;
}

Total operator + (const Total& t, const Entry& e)
{
    QMetaEnum qme = QMetaEnum::fromType<Categories::Type>();
    Total ret;
    ret.setValue(t.value() + (qme.keyToValue(e.type().toLower().toLatin1())*e.value()));
    ret.setDate(Total::maxDate(t.date(), e.date()));

    ret.setValue(round (ret.value() * 100.0) / 100.0);
    return ret;
}

Total operator + (const Entry& e, const Total& t)
{
    return (t + e);
}

Total operator -(const Total& t1, const Total& t2)
{
    Total temp = t2;
    temp.setValue(-t2.value());

    return (t1 +  t2);
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

bool operator <(const Total& t1, const Total& t2)
{
    return (t1.date() < t2.date()) && (t1.value() < t2.value());
}

bool operator == (const Total& t1, const Total& t2)
{
    return (t1.date() == t2.date()) && (t1.value() == t2.value());
}
