#ifndef BUDGET_H
#define BUDGET_H

//TODO v2
#include <QObject>
#include "total.h"

class Budget: public QObject
{
    Q_OBJECT

public:
    enum class BudgetFrequency {unique, day, week, month, quarter, year};
    Q_ENUM(BudgetFrequency)

private:
    Total m_current;
    double m_target;
    BudgetFrequency m_frequency;
    QDate m_start;

public:
    Budget();
    Budget(const Budget&);

    double target() const;
    void setTarget(double);
    BudgetFrequency frequency() const;
    void setFrequency(BudgetFrequency);
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
