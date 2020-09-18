#ifndef DEBT_H
#define DEBT_H

#include "../account_global.h"
#include "accountglobal.h"
#include "entry.h"
#include "frequency.h"
#include "metadata.h"
#include <QDebug>
#include <QtCore/qglobal.h>

class ACCOUNT_EXPORT Debt : public MetaData
{
    Q_GADGET

    Q_PROPERTY(QUuid id READ id CONSTANT)
    Q_PROPERTY(int time READ nb CONSTANT)
    Q_PROPERTY(Account::FrequencyEnum freq READ freq CONSTANT)
    Q_PROPERTY(double rate READ rate CONSTANT)
    Q_PROPERTY(Entry initial READ initial CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QVariantList entries READ variantEntries CONSTANT)
public:
    Debt() = default;
    Debt(const Debt &) = default;
    ~Debt() = default;

    Debt &operator=(const Debt &);

    QUuid id() const;
    void setId(QUuid);
    QString name() const;
    void setName(QString);
    Entry initial() const;
    void setInitial(Entry);
    double rate() const;
    void setRate(double);
    int nb() const;
    void setNb(int);
    Account::FrequencyEnum freq() const;
    void setFreq(Account::FrequencyEnum);

    bool generate();

    QList<Entry> entries() const;
    QVariantList variantEntries() const;
    void setEntries(QList<Entry>);

    Debt &operator<<(Entry);
    friend bool ACCOUNT_EXPORT operator<(const Debt &, const Debt &);
    friend bool ACCOUNT_EXPORT operator==(const Debt &, const Debt &);
};

#endif // DEBT_H
