#ifndef SUBBUDGET_H
#define SUBBUDGET_H

#include "account_global.h"
#include "entry.h"
#include "metadata.h"
#include <QDate>
#include <QMap>

class ACCOUNT_EXPORT SubBudget : public MetaData
{
private:
    QMap<QUuid, double> m_values;

    void update();
public:
    SubBudget();
    SubBudget(const SubBudget&);

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

#endif // SUBBUDGET_H
