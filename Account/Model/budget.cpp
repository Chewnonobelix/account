#include "budget.h"

Budget::Budget()
{
    //TODO
}

Budget::Budget(const Budget & b)
{
//TODO
}

int Budget::id() const
{
//TODO
    return -2;
}

void Budget::setId(int)
{
//TODO
}


bool Budget::addEntry(Entry)
{
//TODO
    return false;
}

bool Budget::removeEntry(Entry)
{
//TODO
    return false;
}

bool Budget::updateEntry(Entry)
{
//TODO
    return false;
}

bool Budget::addTarget(QDate, double)
{
//TODO
    return false;
}

bool Budget::removeTarget(QDate)
{
//TODO
    return false;
}

bool Budget::createSub(QDate)
{
//TODO
    return false;
}

double Budget::current(QDate)
{
//TODO
    return 0;
}

Account::FrequencyEnum Budget::frequency() const
{
//TODO
    return Account::FrequencyEnum::unique;
}

void Budget::setFrequency(Account::FrequencyEnum)
{
//TODO
}

QString Budget::category() const
{
//TODO
    return QString();
}

void Budget::setCategory(QString)
{
//TODO
}

Budget& Budget::operator = (const Budget&)
{
//TODO
    return *this;
}

Budget& Budget::operator <<(Entry)
{
//TODO
    return *this;
}

Budget& Budget::operator >>(Entry)
{
//TODO
    return *this;
}

