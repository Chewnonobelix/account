#include "debt.h"

QUuid Debt::id() const
{
    return QUuid();
}

void Debt::setId(QUuid) {}

Entry Debt::initial() const
{
    return Entry();
}

void Debt::setInitial(Entry) {}

Frequency Debt::scheduler() const
{
    return Frequency();
}

void Debt::setScheduler(Frequency) {}

double Debt::rate() const
{
    return 0.0;
}

void Debt::setRate(double) {}

int Debt::nb() const
{
    return 0;
}

void Debt::setNb(int) {}

Entry Debt::last() const
{
    return Entry();
}

void Debt::setLast(Entry) {}

bool Debt::generate()
{
    return false;
}

bool operator<(const Debt &, const Debt &)
{
    return false;
}

bool operator==(const Debt &, const Debt &)
{
    return false;
}
