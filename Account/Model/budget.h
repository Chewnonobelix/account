#ifndef BUDGET_H
#define BUDGET_H

//TODO v2
#include <QObject>
#include <QSet>
#include <QDebug>
#include "total.h"
#include "accountglobal.h"

class Budget
{
public:

private:
    int m_id;
    Total m_current;
    double m_target;
    Account::FrequencyEnum m_frequency;
    QDate m_start;
    QSet<int> m_related;
    QString m_category;

public:
    Budget();
    Budget(const Budget&);

    int id() const;
    void setId(int);
    double target() const;
    void setTarget(double);
    Account::FrequencyEnum  frequency() const;
    void setFrequency(Account::FrequencyEnum );
    QDate start() const;
    void setStart(QDate);
    double current() const;

    QDate next(QDate) const;
    QDate prev(QDate) const;
    QString category() const;
    void setCategory(QString);
    bool in(QDate) const;
    friend Budget& operator <<(Budget&, Entry&);
    friend Budget& operator >>(Budget&, Entry&);
    Budget& operator = (const Budget&);

    Budget clone(QDate) const;

};

class OverBudget
{
private:
    int m_id;
    double m_target;
    Account::FrequencyEnum m_frequency;
    QDate m_start;
    QSet<int> m_sub;
    QString m_category;

    OverBudget();
    Budget createSub(QDate);

public:
    OverBudget(const OverBudget&);
    ~OverBudget();

    OverBudget& operator =(const OverBudget&);
    bool add(Entry&);
    bool remove(Entry&);
    static OverBudget create(QString);

    int id() const;
    double target() const;
    void setTarget(double);
    Account::FrequencyEnum frequency() const;
    void setFrequency(const Account::FrequencyEnum);
    QDate start() const;
    void setStart(QDate);
    QString category() const;
    void setCategory(QString);
    void setId(int id);
};

#endif // BUDGET_H
