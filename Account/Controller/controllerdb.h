#ifndef CONTROLLERDB_H
#define CONTROLLERDB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSharedPointer>
#include "../Model/entry.h"

class ControllerDB
{
    typedef QSharedPointer<QSqlQuery> SqlQuery;

private:
    QSqlDatabase m_db;
    SqlQuery m_addEntry;
    QString m_removeEntry;

    SqlQuery m_selectEntry;
    SqlQuery m_accounts;

    SqlQuery m_updateInfo;
    SqlQuery m_addInformation;


public:
    ControllerDB();
    ~ControllerDB();

    bool isConnected() const;

    bool addEntry(const Entry&);
    QList<Entry> selectEntry(QString);
    QStringList selectAccount();
    bool updateInfo(const Entry&);
};


#endif // CONTROLLERDB_H
