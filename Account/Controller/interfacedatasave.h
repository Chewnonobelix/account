#ifndef INTERFACEDATASAVE_H
#define INTERFACEDATASAVE_H

#include <QString>
#include <QList>
#include "Model/entry.h"
#include "Model/budget.h"
#include "Model/accountglobal.h"

class InterfaceDataSave
{
public:
    virtual ~InterfaceDataSave() {}
    virtual bool addEntry(const Entry&) = 0;
    virtual QList<Entry> selectEntry(QString) = 0;
    virtual bool removeEntry(const Entry&) = 0;

    virtual QStringList selectAccount() = 0;
    virtual bool removeAccount(QString) = 0;

    virtual bool updateInfo(const Entry&) = 0;
    virtual bool updateEntry(const Entry&) = 0;

    virtual bool addCategory(QString, QString) = 0;
    virtual bool removeCategory(QString) = 0;
    virtual QMultiMap<QString, QString> selectCategory() = 0;

    virtual bool addBudget(const Budget&) = 0;
    virtual bool removeBudget(const Budget&) = 0;
    virtual QList<Budget> selectBudgets() = 0;
    virtual bool updateBudget(const Budget&) = 0;

    virtual bool init() = 0;
};

#endif // INTERFACEDATASAVE_H
