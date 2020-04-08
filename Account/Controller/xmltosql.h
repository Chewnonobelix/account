#ifndef XMLTOSQL_H
#define XMLTOSQL_H

#include <QProcess>
#include "abstractcontroller.h"
#include "controllerdb.h"
#include "controllerxmlmulti.h"

class TransfertDatabase
{
private:
    InterfaceDataSave* m_backup;
    InterfaceDataSave* m_db;

    bool transfertEntries();
    bool transfertCategories();
    bool transfertBudget();
    bool transfertFrequency();
    bool transfertCommon();

    QStringList accounts;
    QString backuptype;
public:
    TransfertDatabase(InterfaceDataSave*, InterfaceDataSave*);

    int exec();
};

#endif // XMLTOSQL_H
