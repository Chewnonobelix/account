#include "debt.h"

QUuid Debt::id() const
{
    return metaData<QUuid>("id");
}

void Debt::setId(QUuid id)
{
    setMetadata("id", id);
}

QString Debt::name() const
{
    return metaData<QString>("name");
}

void Debt::setName(QString n)
{
    setMetadata("name", n);
}

Entry Debt::initial() const
{
    return metaData<Entry>("initial");
}

void Debt::setInitial(Entry i)
{
    setMetadata("initial", i);
}

Frequency Debt::scheduler() const
{
    return metaData<Frequency>("scheduler");
}

void Debt::setScheduler(Frequency s)
{
    setMetadata("scheduler", s);
}

double Debt::rate() const
{
    return metaData<double>("rate");
}

void Debt::setRate(double rate)
{
    setMetadata("rate", rate);
}

int Debt::nb() const
{
    return metaData<int>("nb");
}

void Debt::setNb(int n)
{
    setMetadata("nb", n);
}

bool Debt::generate()
{
    return false;
}

bool operator<(const Debt &d1, const Debt &d2)
{
    return (d1.name() < d2.name()) || (d1.id() < d2.id());
}

bool operator==(const Debt &d1, const Debt &d2)
{
    return d1.id() == d2.id();
}

QList<Entry> Debt::entries() const
{
    return metaData<QList<Entry>>("entries");
}

void Debt::setEntries(QList<Entry> e)
{
    setMetadata("entries", e);
}
