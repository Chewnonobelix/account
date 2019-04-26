#ifndef BUDGET_H
#define BUDGET_H

//TODO v2
#include <QObject>
#include <QSet>
#include <QMap>
#include <QList>
#include <QDebug>
#include "total.h"
#include "accountglobal.h"
#include "subbudget.h"


class Budget
{
public:

private:
    int m_id;
    QString m_category;
    Account::FrequencyEnum m_frequency;
    QMap<QDate, double> m_targets;
    QMap<QDate, SubBudget> m_subs;

    QDate next(QDate) const;
    QDate previous(QDate) const;

public:
    Budget();
    Budget(const Budget&);

    int id() const;
    void setId(int);

    bool addEntry(Entry);
    bool removeEntry(Entry);
    bool updateEntry(Entry);

    bool addTarget(QDate, double);
    bool removeTarget(QDate);
    bool updateTarget(QDate, double);

    bool createSub(QDate);
    double current(QDate);

    Account::FrequencyEnum frequency() const;
    void setFrequency(Account::FrequencyEnum);

    QString category() const;
    void setCategory(QString);

    Budget& operator = (const Budget&);
    Budget& operator <<(Entry);
    Budget& operator >>(Entry);
};

#endif // BUDGET_H
