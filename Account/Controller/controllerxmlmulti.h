#ifndef CONTROLLERXMLMULTI_H
#define CONTROLLERXMLMULTI_H

#include <QDir>
#include <QDebug>
#include "controllerxml.h"

class ControllerXMLMulti: public InterfaceDataSave, public QObject
{

private:
    QMap<QString, QDomDocument> m_accounts;
    QSet<int> m_entriesId;
    QSet<int> m_infoId;
    QDomDocument m_currentAccount;

    int m_timer;

    int maxId(const QSet<int> &) const;
    Information selectInformation(const QDomElement&) const;
    void addInfo(QDomElement&, const Information&);

    void createAccount(QString);

protected:
    void timerEvent(QTimerEvent*);

public:
    ControllerXMLMulti();
    ControllerXMLMulti(const ControllerXMLMulti&);
    ~ControllerXMLMulti();

    void close();

    void setCurrentAccount(QString);

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
};

Q_DECLARE_METATYPE(ControllerXMLMulti)

#endif // CONTROLLERXMLMULTI_H
