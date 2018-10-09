#ifndef CONTROLLERXML_H
#define CONTROLLERXML_H

#include <QDomDocument>
#include <QDomElement>
#include <QSharedPointer>
#include <QFile>
#include <QList>
#include "interfacedatasave.h"

class ControllerXML: public InterfaceDataSave
{
private:
    QString m_filename;
    QDomDocument m_document;
    QFile* m_file;
    Information selectInformation(const QDomElement&) const;
    void addInfo(QDomElement&, const Information&);

    QSet<int> m_entryId;
    QSet<int> m_infoId;
    QSet<QString> m_accountList;

    int maxId(const QSet<int>&) const;

public:
    ControllerXML();
    ControllerXML(const ControllerXML&);
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

    virtual bool init();
};

#endif // CONTROLLERXML_H
