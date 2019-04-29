#ifndef SUBBUDGET_H
#define SUBBUDGET_H

#include <QMap>
#include <QDate>
#include "entry.h"

class SubBudget
{
private:
    double m_current;
    double m_target;
    QDate m_begin;
    QDate m_end;
    QMap<int, double> m_values;

    void update();
public:
    SubBudget();
    SubBudget(const SubBudget&);

    SubBudget& operator = (const SubBudget&);

    double current() const;
    double target() const;
    void setTarget(double);
    QDate begin() const;
    void setBegin(QDate);
    QDate end() const;
    void setEnd(QDate);
    double percent() const;

    bool addEntry(Entry);
    bool removeEntry(Entry);
    bool updateEntry(Entry);

    bool in(QDate) const;
};

#endif // SUBBUDGET_H
