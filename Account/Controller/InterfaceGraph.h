#ifndef INTERFACEGRAPH_H
#define INTERFACEGRAPH_H

#include "Model/entry.h"
#include "Model/accountglobal.h"

class InterfaceGraph
{
public:
    virtual void add(const Entry&) = 0;
    virtual void clear() = 0;
    virtual void setDate(QDate) = 0;
    virtual void setGran(Account::Granularity) = 0;
};

#endif // INTERFACEGRAPH_H
