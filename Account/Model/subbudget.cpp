#include "subbudget.h"

SubBudget::SubBudget()
{

}

SubBudget::SubBudget(const SubBudget&)
{
//TODO
}

SubBudget& SubBudget::operator = (const SubBudget&)
{
    //TODO
    return *this;
}

double SubBudget::current() const
{
    //TODO
    return 0;
}

double SubBudget::target() const
{
    //TODO
    return 0;
}

void SubBudget::setTrget(double)
{
    //TODO
}

QDate SubBudget::begin() const
{
    //TODO
    return QDate();
}

void SubBudget::setBegin(QDate)
{
    //TODO
}

QDate SubBudget::end() const
{
    //TODO
    return QDate();

}

void SubBudget::setEnd(QDate)
{
    //TODO
}

bool SubBudget::addEntry(Entry)
{
    //TODO
    return false;
}

bool SubBudget::removeEntry(Entry)
{
    //TODO
    return false;
}

bool SubBudget::updateEntry(Entry)
{
    //TODO
    return false;
}

