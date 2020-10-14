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

    QMutex m_mutex;
    
    void createAccount(QString);

    void adder(QDomElement&, QString , QString, QMap<QString, QString> = QMap<QString, QString>());
    void setter(QDomElement&, QString , QString, QMap<QString, QString> = QMap<QString, QString>() );
    void deleter(QDomElement&, QString);
    
    bool addEntryNode(Entry&, QDomElement&, QString = "entry" );
    bool updateEntryNode(Entry&, QDomElement&);
    Entry selectEntryNode(QDomElement&);

protected:

public:
    ControllerXMLMulti(bool = false);
    ControllerXMLMulti(const ControllerXMLMulti&);
    ~ControllerXMLMulti();

    void close();


public slots:

    virtual bool addEntry(Entry &) override;
    virtual QMultiMap<QDate, Entry> selectEntry() override;
    virtual bool removeEntry(Entry &) override;

    virtual QStringList selectAccount(QString) override;
    virtual bool removeAccount(QString) override;

    virtual bool updateEntry(Entry &) override;

	virtual bool addCategory(Category &) override;
	virtual bool removeCategory(QString) override;
    virtual QMap<Account::TypeEnum, QMap<QUuid, Category>> selectCategory() override;
    virtual bool updateCategory(Category &) override;

    virtual bool addBudget(Budget &) override;
    virtual bool removeBudget(Budget &) override;
    virtual QMap<QUuid, Budget> selectBudgets() override;
    virtual bool updateBudget(Budget &) override;

    virtual bool addFrequency(Frequency &) override;
    virtual bool removeFrequency(Frequency &) override;
    virtual bool updateFrequency(Frequency &) override;
    virtual QMap<QUuid, Frequency> selectFrequency() override;

    virtual QMap<QUuid, CommonExpanse> selectCommon() override;
    virtual bool addCommon(CommonExpanse &) override;
    virtual bool removeCommon(CommonExpanse &) override;
    virtual bool updateCommon(CommonExpanse &) override;

    virtual QStringList selectProfile() override;
    virtual bool addProfile(QString, QString) override;
    virtual bool deleteProfile(QString) override;

    virtual QMap<QUuid, Debt> selectDebt() override;
    virtual bool addDebt(Debt &) override;
    virtual bool removeDebt(Debt &) override;
    virtual bool updateDebt(Debt &) override;

    virtual bool init() override;
    virtual QSharedPointer<InterfaceDataSave> clone() const override;
};

Q_DECLARE_METATYPE(ControllerXMLMulti)

#endif // CONTROLLERXMLMULTI_H
