#ifndef CONTROLLERDB_H
#define CONTROLLERDB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlRecord>
#include "abstractcontroller.h"

class ControllerDB
{
private:
    QSqlQuery m_addEntry;
    QString m_removeEntry;
    QSqlQuery m_selectEntry;
    QSqlQuery m_accounts;

public:
    ControllerDB();

    bool addEntry(const Entry&);
    QList<Entry> selectEntry(QString);
    QStringList selectAccount();
};

#endif // CONTROLLERDB_H
