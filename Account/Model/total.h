#ifndef TOTAL_H
#define TOTAL_H

#include <QDate>
#include <QString>
#include "entry.h"

class Total
{
private:
    QDate m_date;
    double m_value;

public:
    Total();
    Total(const Total&);
    ~Total();

    Total& operator = (const Total&);

    friend Total operator+(const Entry&, const Entry&);
    friend Total operator + (const Total&, const Total&);
    friend Total operator + (const Total&, const Entry&);
    friend Total operator + (const Entry&, const Total&);
    friend bool operator < (const Total&, const Total&);
    friend bool operator ==(const Total&, const Total&);

    QDate date() const;
    void setDate(QDate date);
    double value() const;
    void setValue(double value);

    static QDate maxDate(const QDate&, const QDate&);
};

#endif // TOTAL_H
