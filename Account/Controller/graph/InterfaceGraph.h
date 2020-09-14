#ifndef INTERFACEGRAPH_H
#define INTERFACEGRAPH_H

#include <QQmlApplicationEngine>
#include "Model/entry.h"
#include "Model/accountglobal.h"

class InterfaceGraph
{
public:
    virtual void add(const Entry&) = 0;
    virtual void setView(const QQmlApplicationEngine&) = 0;
    virtual void update() = 0;
    virtual void clear() = 0;
    virtual void setGran(Account::Granularity) = 0;
};

#endif // INTERFACEGRAPH_H
