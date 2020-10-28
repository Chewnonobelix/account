#include "total.h"
#include "accountglobal.h"


#include <QDebug>

using namespace Account;
Total::Total() : MetaData() {}

Total::Total(const Total &t) : MetaData(t) {}

Total& Total::operator = (const Total& t)
{
 MetaData::operator=(t);
 return *this;
}

Total operator+(const Entry& e1, const Entry& e2)
{
    Total ret;

    ret.setValue((int(e1.type()) * e1.value()) + (int(e1.type()) * e2.value()));
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
    Total ret;
    ret.setValue(t.value() + (int(e.type())) * e.value());
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

    return (t1 +  temp);
}

QDate Total::date() const
{
 return metaData<QDate>("date");
}

void Total::setDate(QDate date)
{
 setMetadata("date", date);
}

double Total::value() const
{
 return metaData<double>("value");
}

void Total::setValue(double value)
{
 setMetadata("value", value);
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
