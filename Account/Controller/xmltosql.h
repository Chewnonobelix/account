#ifndef XMLTOSQL_H
#define XMLTOSQL_H

#include "abstractcontroller.h"
#include "controllerdb.h"
#include "controllerxmlmulti.h"

class TransfertDatabase: public AbstractController
{
    InterfaceDataSave* m_backup;

    bool transfertEntries();
    bool transfertCategories();
    bool transfertBudget();
    bool transfertFrequency();
    bool transfertCommon();

    QStringList accounts;

public:
    TransfertDatabase(InterfaceDataSave*);

    int exec();
};

#endif // XMLTOSQL_H
