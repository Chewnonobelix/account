#ifndef XMLTOSQL_H
#define XMLTOSQL_H

#include <QProcess>
#include "abstractcontroller.h"
#include "controllerdb.h"
#include "controllerxmlmulti.h"

class TransfertDatabase: public QThread
{
private:

    bool transfertEntries();
    bool transfertCategories();
    bool transfertBudget();
    bool transfertFrequency();
    bool transfertCommon();

    QStringList accounts;
    QString backuptype;

    bool m_sucess = true;

protected:
    void run() override;

public:
    TransfertDatabase();
    inline bool isSucess() {return m_sucess;}
    InterfaceDataSave* m_backup;
    InterfaceDataSave* m_db;


};

#endif // XMLTOSQL_H
