#ifndef CONTROLLERXMLMULTI_H
#define CONTROLLERXMLMULTI_H

#include <QDir>
#include "controllerxml.h"

class ControllerXMLMulti: public ControllerXML
{
private:
    QString m_owner;
    QMap<QString, QDomDocument> m_accounts;
    QSet<int> m_entriesId;
    QSet<int> m_infoId;

    int maxId(const QSet<int> &) const;
    Information selectInformation(const QDomElement&) const;

public:
    ControllerXMLMulti();
    ControllerXMLMulti(const ControllerXMLMulti&);
    ~ControllerXMLMulti();


    virtual bool addEntry(const Entry&);
    virtual QList<Entry> selectEntry(QString);
    virtual bool removeEntry(const Entry&);

    virtual QStringList selectAccount();
    virtual bool removeAccount(QString);

    virtual bool updateInfo(const Entry&);
    virtual bool updateEntry(const Entry &);

    virtual bool addCategory(QString, QString);
    virtual bool removeCategory(QString);
    virtual QMap<QString, QString> selectCategory();


    virtual bool init();

    QString owner() const;
    void setOwner(QString);
};

Q_DECLARE_METATYPE(ControllerXMLMulti)

#endif // CONTROLLERXMLMULTI_H
