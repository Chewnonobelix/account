#ifndef XMLTOSQL_H
#define XMLTOSQL_H

#include "controllerdb.h"
#include "controllerxmlmulti.h"

class XmltoSql
{
    ControllerDB m_db;
    ControllerXMLMulti& m_xml;

    bool transfertEntries();
    bool transfertcategories();

public:
    XmltoSql(ControllerXMLMulti&);

    bool exec();
};

#endif // XMLTOSQL_H
