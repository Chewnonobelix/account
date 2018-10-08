#ifndef CONTROLLERXML_H
#define CONTROLLERXML_H

#include <QDomDocument>
#include <QDomElement>

#include "interfacedatasave.h"

class ControllerXML
{
private:
    QString m_filename;
    QDomDocument m_document;

public:
    ControllerXML();
    ~ControllerXML();

    void setFilename(QString);

    virtual bool addEntry(const Entry&);
    virtual QList<Entry> selectEntry(QString);
    virtual bool removeEntry(const Entry&);

    virtual QStringList selectAccount();
    virtual bool removeAccount(QString);

    virtual bool updateInfo(const Entry&);

    virtual bool addCategory(QString, Categories::Type);
    virtual bool removeCategory(QString);
};

#endif // CONTROLLERXML_H
