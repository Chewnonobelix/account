#ifndef BUDGET_H
#define BUDGET_H

//TODO v2
#include <QObject>
#include <QSet>
#include <QMap>
#include <QList>
#include <QDebug>
#include "total.h"
#include "accountglobal.h"

class Budget
{
public:

private:
    int m_id;

public:
    Budget();
    Budget(const Budget&);

    int id() const;
    void setId(int);
    Budget& operator = (const Budget&);
};

#endif // BUDGET_H
