#ifndef CONTROLLERDB_H
#define CONTROLLERDB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlRecord>
#include "../Model/entry.h"

class ControllerDB
{
private:
    QSqlDatabase m_db;
    QSqlQuery* m_addEntry;
    QString m_removeEntry;
    QSqlQuery* m_selectEntry;
    QSqlQuery* m_accounts;

public:
    ControllerDB();
    ~ControllerDB();

    bool isConnected() const;

    bool addEntry(const Entry&);
    QList<Entry> selectEntry(QString);
    QStringList selectAccount();
};

#endif // CONTROLLERDB_H
