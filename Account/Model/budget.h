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
#include <QJsonArray>

class Target {
	Q_GADGET

	Q_PROPERTY(double target MEMBER target)
	Q_PROPERTY(Account::FrequencyEnum frequency MEMBER frequency)
	Q_PROPERTY(QDate date MEMBER date)

	public:
	QDate date;
	double target;
	Account::FrequencyEnum frequency;

	operator QJsonObject() const;
	Target(const QJsonObject&);
	Target(QDate, double, Account::FrequencyEnum);
	Target() = default;
	Target(const Target&) = default;
};

class ACCOUNT_EXPORT Budget: public MetaData
{
	Q_GADGET

	Q_PROPERTY(Category category READ category WRITE setCategory)
	Q_PROPERTY(Account::TypeEnum type READ type WRITE setType)

public:



private:
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

				bool addTarget(QDate, double, Account::FrequencyEnum f);
    bool removeTarget(QDate);
    bool updateTarget(QDate, double);
				QMap<QDate, Target> targets() const;

    bool createSub(QDate);
    double current(QDate);

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
