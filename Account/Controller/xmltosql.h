#ifndef XMLTOSQL_H
#define XMLTOSQL_H

#include "controllerdb.h"
#include "controllerxml.h"

class XmltoSql
{
    ControllerDB m_db;
    ControllerXML& m_xml;

    bool transfertEntries();
    bool transfertcategories();

public:
    XmltoSql(ControllerXML&);

    bool exec();
};

#endif // XMLTOSQL_H
