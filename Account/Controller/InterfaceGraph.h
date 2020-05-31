#ifndef INTERFACEGRAPH_H
#define INTERFACEGRAPH_H

#include "Model/entry.h"

class InterfaceGraph
{
public:
    virtual void change(int) = 0;
    virtual void increment(int = 0) = 0;
    virtual void add(const Entry&) = 0;
    virtual void clear() = 0;
};

#endif // INTERFACEGRAPH_H
