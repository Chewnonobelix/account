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

Account::FrequencyEnum Debt::freq() const
{
    return metaData<Account::FrequencyEnum>("freq");
}

void Debt::setFreq(Account::FrequencyEnum n)
{
    setMetadata("freq", n);
}

bool Debt::generate()
{
    double multrate = rate() + 1;
    double vt = initial().value();

    int value = std::floor(vt / nb() * multrate * 100);
    int lastvalue = ((int) (vt * multrate * 100)) - nb() * value;

    Entry ref;
    ref.setValue(value / 100.0);
    ref.setType(initial().type() == "income" ? "outcome" : "income");
    QDate first = initial().date();
    QDate last = first.addDays((nb() + 1) * Account::nbDay(first, freq()));

    Frequency scheduler;
    scheduler.setFreq(freq());
    scheduler.setReferenceEntry(ref);

    QList<Entry> list;
    for (first; first < last; first = first.addDays(Account::nbDay(first, freq()))) {
        list << scheduler.clone(ref);
        list.last().setDate(first);
    }

    list.last().setValue(lastvalue / 100.0);

    setEntries(list);

    return true;
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
