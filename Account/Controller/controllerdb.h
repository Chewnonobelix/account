#ifndef CONTROLLERDB_H
#define CONTROLLERDB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSharedPointer>
#include <QSqlError>
#include "../Model/entry.h"
#include "interfacedatasave.h"

class ControllerDB: public InterfaceDataSave
{
    typedef QSharedPointer<QSqlQuery> SqlQuery;
    Q_OBJECT

private:
    QSqlDatabase m_db;

    SqlQuery m_addEntry;
    SqlQuery m_removeEntry;
    SqlQuery m_selectEntry;
    SqlQuery m_updateEntry; //TODO

    SqlQuery m_accounts;
    SqlQuery m_removeAccount; //TODO

    SqlQuery m_updateInfo;
    SqlQuery m_addInformation;
    SqlQuery m_removeInformation;
    SqlQuery m_selectInformation;

    SqlQuery m_addCategory;
    SqlQuery m_removeCategory;
    SqlQuery m_selectCategory;

public:
    ControllerDB();
    ControllerDB(const ControllerDB&d);
    ~ControllerDB();

    bool isConnected() const;
public slots:
    bool addEntry(QSharedPointer<Entry>) {return false;}
    bool addEntry(const Entry&);
    QMultiMap<QDate, Entry> selectEntry(QString);
    bool removeEntry(const Entry&);

    QStringList selectAccount();
    bool removeAccount(QString);

    bool updateInfo(const Entry&);
    bool updateEntry(const Entry &);

    bool addCategory(QString, QString);
    bool removeCategory(QString);
    QMultiMap<QString, QString> selectCategory();

    virtual bool addBudget(const Budget&);
    virtual bool removeBudget(const Budget&);
    virtual QList<Budget> selectBudgets();
    virtual bool updateBudget(const Budget &);

    virtual bool addFrequency(const Frequency&);
    virtual bool removeFrequency(const Frequency&);
    virtual bool updateFrequency(const Frequency&);
    virtual QList<Frequency> selectFrequency();
    
    virtual QMap<int, CommonExpanse> selectCommon() {return  QMap<int, CommonExpanse>(); }
    virtual bool addCommon(const CommonExpanse&) {return false;}
    virtual bool removeCommon(const CommonExpanse&) {return false;}
    virtual bool updateCommon(const CommonExpanse&) {return false;}
    
    bool init();
};

Q_DECLARE_METATYPE(ControllerDB)

#endif // CONTROLLERDB_H
