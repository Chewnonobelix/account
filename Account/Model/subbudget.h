#ifndef SUBBUDGET_H
#define SUBBUDGET_H

#include "account_global.h"
#include "entry.h"
#include "metadata.h"
#include <QDate>
#include <QMap>

class ACCOUNT_EXPORT SubBudget : public MetaData
{
	Q_GADGET

	Q_PROPERTY(QDate begin READ begin)
	Q_PROPERTY(QDate end READ end)
	Q_PROPERTY(double target READ target)
	Q_PROPERTY(double current READ current)
//	Q_PROPERTY(QString cat READ cat)

private:
    QMap<QUuid, double> m_values;

    void update();
public:
    SubBudget();
    SubBudget(const SubBudget&);
    ~SubBudget() = default;
    using MetaData::MetaData;

    SubBudget& operator = (const SubBudget&);

    QDate reference() const;
    void setReference(QDate);
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

Q_DECLARE_METATYPE(SubBudget)

#endif // SUBBUDGET_H
