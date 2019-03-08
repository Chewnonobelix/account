#ifndef BUDGET_H
#define BUDGET_H

//TODO v2
#include <QObject>
#include <QSet>
#include "total.h"
#include "accountglobal.h"

class Budget
{
public:

private:
    Total m_current;
    double m_target;
    Account::FrequencyEnum m_frequency;
    QDate m_start;
    QSet<int> m_related;

public:
    Budget();
    Budget(const Budget&);

    double target() const;
    void setTarget(double);
    Account::FrequencyEnum  frequency() const;
    void setFrequency(Account::FrequencyEnum );
    QDate start() const;
    void setStart(QDate);

    QDate next(QDate) const;
    QDate prev(QDate) const;
    bool in(QDate) const;
    friend Budget& operator +(Budget&, Entry);
    friend Budget& operator -(Budget&, Entry);

    Budget clone(QDate) const;

};

#endif // BUDGET_H
