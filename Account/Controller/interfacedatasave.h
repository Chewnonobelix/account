#ifndef INTERFACEDATASAVE_H
#define INTERFACEDATASAVE_H

#include <QString>
#include <QList>
#include "Model/entry.h"
#include "Model/budget.h"

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
    virtual QMap<QString, QString> selectCategory() = 0;

    virtual bool addBudget(QString) = 0;
    virtual bool removeBudget(QString) = 0;
    virtual QList<Budget> selectBudgets() = 0;

    virtual bool init() = 0;
};

#endif // INTERFACEDATASAVE_H
