#ifndef CONTROLLERDB_H
#define CONTROLLERDB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSharedPointer>
#include <QSqlError>
#include "../Model/entry.h"

class ControllerDB
{
    typedef QSharedPointer<QSqlQuery> SqlQuery;

private:
    QSqlDatabase m_db;

    SqlQuery m_addEntry;
    SqlQuery m_removeEntry;
    SqlQuery m_selectEntry;

    SqlQuery m_accounts;

    SqlQuery m_updateInfo;
    SqlQuery m_addInformation;
    SqlQuery m_removeInformation;
    SqlQuery m_selectInformation;


public:
    ControllerDB();
    ~ControllerDB();

    bool isConnected() const;

    bool addEntry(const Entry&);
    QList<Entry> selectEntry(QString);
    bool removeEntry(const Entry&);

    QStringList selectAccount();

    bool updateInfo(const Entry&);
};


#endif // CONTROLLERDB_H
