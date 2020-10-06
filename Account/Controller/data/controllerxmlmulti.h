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

    //    QMap<QString, QSet<int>> m_ids;

    //    QDomDocument m_currentAccount;
    //    QString m_currentProfile;

    QMutex m_mutex;
    
    int maxId(const QSet<int> &) const;

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

    virtual bool addEntry(const Entry &) override;
    virtual QMultiMap<QDate, Entry> selectEntry() override;
    virtual bool removeEntry(const Entry &) override;

    virtual QStringList selectAccount(QString) override;
    virtual bool removeAccount(QString) override;

    virtual bool updateEntry(const Entry &) override;

    virtual bool addCategory(const Category &) override;
    virtual bool removeCategory(QString) override;
    virtual QMultiMap<Account::TypeEnum, Category> selectCategory() override;
    virtual bool updateCategory(const Category &) override;

    virtual bool addBudget(const Budget &) override;
    virtual bool removeBudget(const Budget &) override;
    virtual QList<Budget> selectBudgets() override;
    virtual bool updateBudget(const Budget &) override;

    virtual bool addFrequency(const Frequency &) override;
    virtual bool removeFrequency(const Frequency &) override;
    virtual bool updateFrequency(const Frequency &) override;
    virtual QList<Frequency> selectFrequency() override;

    virtual QMap<QUuid, CommonExpanse> selectCommon() override;
    virtual bool addCommon(const CommonExpanse &) override;
    virtual bool removeCommon(const CommonExpanse &) override;
    virtual bool updateCommon(const CommonExpanse &) override;

    virtual QStringList selectProfile() override;
    virtual bool addProfile(QString, QString) override;
    virtual bool deleteProfile(QString) override;

    virtual QMap<QUuid, Debt> selectDebt() override;
    virtual bool addDebt(const Debt &) override;
    virtual bool removeDebt(const Debt &) override;
    virtual bool updateDebt(const Debt &) override;

    virtual bool init() override;
    virtual QSharedPointer<InterfaceDataSave> clone() const override;
};

Q_DECLARE_METATYPE(ControllerXMLMulti)

#endif // CONTROLLERXMLMULTI_H
