#ifndef DEBT_H
#define DEBT_H

#include "../account_global.h"
#include "accountglobal.h"
#include "entry.h"
#include "frequency.h"
#include "metadata.h"
#include <QtCore/qglobal.h>

class ACCOUNT_EXPORT Debt : public MetaData
{
public:
    Debt() = default;
    Debt(const Debt &) = default;
    ~Debt() = default;

    Debt &operator=(const Debt &) = default;

    QUuid id() const;
    void setId(QUuid);
    QString name() const;
    void setName(QString);
    Entry initial() const;
    void setInitial(Entry);
    Frequency scheduler() const;
    void setScheduler(Frequency);
    double rate() const;
    void setRate(double);
    int nb() const;
    void setNb(int);

    bool generate();

    QList<Entry> entries() const;
    void setEntries(QList<Entry>);

    friend bool ACCOUNT_EXPORT operator<(const Debt &, const Debt &);
    friend bool ACCOUNT_EXPORT operator==(const Debt &, const Debt &);
};

#endif // DEBT_H
