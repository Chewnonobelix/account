#ifndef BUDGET_H
#define BUDGET_H

#include <QObject>
#include <QSet>
#include <QMap>
#include <QList>
#include <QDebug>
#include <QUuid>
#include "total.h"
#include "accountglobal.h"
#include "subbudget.h"
#include "metadata.h"

class ACCOUNT_EXPORT Budget: public MetaData
{
public:

				class Target {
					Q_GADGET

					Q_PROPERTY(double target MEMBER target)
					Q_PROPERTY(Account::FrequencyEnum frequency MEMBER frequency)
					Q_PROPERTY(QDate date MEMBER frequency)

					public:
									QDate date;
									double target;
									Account::FrequencyEnum frequency;
				};

private:
				QMap<QDate, Target> m_targets;
    QMap<QDate, SubBudget> m_subs;

    QMap<QDate, SubBudget>::iterator m_subit = m_subs.end();
    
    QDate next(QDate) const;
    QDate previous(QDate) const;

public:
    Budget();
    Budget(const Budget &);
    ~Budget() = default;
    Budget(const QJsonObject&);
    operator QJsonObject() const;

    QUuid id() const;
    void setId(QUuid);

    Account::TypeEnum type() const;
    void setType(Account::TypeEnum);

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

    Category category() const;
    void setCategory(Category);

    QDate reference() const;
    void setReference(QDate);

    QMap<QDate, SubBudget> subs() const;

    Budget& operator = (const Budget&);
    Budget& operator <<(Entry);
    Budget& operator >>(Entry);
};

#endif // BUDGET_H
