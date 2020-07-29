#ifndef BUDGET_H
#define BUDGET_H

#include <QObject>
#include <QSet>
#include <QMap>
#include <QList>
#include <QDebug>
#include "total.h"
#include "accountglobal.h"
#include "subbudget.h"
#include "metadata.h"

class ACCOUNT_EXPORT Budget: public MetaData
{
public:

private:
    int m_id;

    QString m_category;
    QDate m_reference;

    QMap<QDate, Account::FrequencyEnum> m_frequency;
    QMap<QDate, double> m_targets;
    QMap<QDate, SubBudget> m_subs;

    QMap<QDate, SubBudget>::iterator m_subit = m_subs.end();
    
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
    QMap<QDate, double> targets() const;

    bool createSub(QDate);
    double current(QDate);

    Account::FrequencyEnum frequency(QDate) const;
    void setFrequency(QDate, Account::FrequencyEnum);

    QString category() const;
    void setCategory(QString);

    QDate reference() const;
    void setReference(QDate);

    QMap<QDate, SubBudget> subs() const;

    Budget& operator = (const Budget&);
    Budget& operator <<(Entry);
    Budget& operator >>(Entry);
};

#endif // BUDGET_H
