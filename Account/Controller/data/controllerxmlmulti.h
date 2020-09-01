#ifndef CONTROLLERXMLMULTI_H
#define CONTROLLERXMLMULTI_H

#include <QDir>
#include <QDebug>
#include <QMutex>
#include <QDomDocument>
#include <QXmlQuery>
#include <QXmlSerializer>
#include <QBuffer>
#include <QXmlResultItems>
#include "interfacedatasave.h"

class ControllerXMLMulti: public InterfaceDataSave
{
    Q_OBJECT
private:
    QMap<QString, QDomDocument> m_accounts;
    
    QMap<QString, QSet<int>> m_ids;

    QDomDocument m_currentAccount;
    QString m_currentProfile;

    QMutex m_mutex;
    
    int maxId(const QSet<int> &) const;
    Information selectInformation(const QDomElement&) const;
    void addInfo(QDomElement&, const Information&);
    void updateInfo(QDomElement&, const Information&);

    void createAccount(QString);

    void adder(QDomElement&, QString , QString, QMap<QString, QString> = QMap<QString, QString>());
    void setter(QDomElement&, QString , QString, QMap<QString, QString> = QMap<QString, QString>() );
    void deleter(QDomElement&, QString);
    
    bool addEntryNode(const Entry&, QDomElement&, QString = "entry" );
    bool updateEntryNode(const Entry&, QDomElement&);
    Entry selectEntryNode(QDomElement&);

protected:

public:
    ControllerXMLMulti(bool = false);
    ControllerXMLMulti(const ControllerXMLMulti&);
    ~ControllerXMLMulti();

    void close();


public slots:

    virtual bool addEntry(QSharedPointer<Entry>);
    virtual bool addEntry(const Entry&);
    virtual QMultiMap<QDate, Entry> selectEntry(QString = "");
    virtual bool removeEntry(const Entry&);

    virtual QStringList selectAccount();
    virtual bool removeAccount(QString);
    virtual void setCurrentAccount(QString);

    virtual bool updateInfo(const Entry&);
    virtual bool updateEntry(const Entry &);

    virtual bool addCategory(QString, QString);
    virtual bool removeCategory(QString);
    virtual QMultiMap<QString, QString> selectCategory();

    virtual bool addBudget(const Budget &);
    virtual bool removeBudget(const Budget&);
    virtual QList<Budget> selectBudgets();
    virtual bool updateBudget(const Budget &);
    
    virtual bool addFrequency(const Frequency&);
    virtual bool removeFrequency(const Frequency&);
    virtual bool updateFrequency(const Frequency&);
    virtual QList<Frequency> selectFrequency();
    
    virtual QMap<QUuid, CommonExpanse> selectCommon();
    virtual bool addCommon(const CommonExpanse&);
    virtual bool removeCommon(const CommonExpanse&);
    virtual bool updateCommon(const CommonExpanse&);
    
    virtual QStringList selectProfile();
    virtual void setProfile(QString);
    virtual bool addProfile(QString, QString);
    virtual QString currentProfile();
    virtual bool deleteProfile(QString);
    
    virtual bool init();
};

Q_DECLARE_METATYPE(ControllerXMLMulti)

#endif // CONTROLLERXMLMULTI_H
