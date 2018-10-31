#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QString>
#include <QDate>
#include <QMultiMap>
#include <QObject>
#include <QDebug>
#include <QRandomGenerator>
#include "Model/entry.h"
#include "Model/categories.h"
#include "Model/total.h"

#include "interfacedatasave.h"

class AbstractController: public QObject
{
    Q_OBJECT

private:
    static QString m_account;
    static QMultiMap<QDate, Entry> m_entry;
    static InterfaceDataSave* m_db;


public:
    AbstractController();
    ~AbstractController();

    virtual int exec() = 0;

    static void setAccountList(QStringList);
    static QStringList accountList();

    static void setCurrentAccount(QString);
    static QString currentAccount();

    static void addEntry(const Entry&);
    static void removeEntry(const Entry&);
    static void updateEntry(const Entry&);

    static Entry entry(int);
    static QList<Entry> entries(QDate = QDate());

    static void setDb(QString);
    static void deleteDb();

    static void addCategory(QString, QString);
    static QStringList categories(QString);

    static void deletAccount(QString);

    static void initTestEntry();
};

#endif // ABSTRACTCONTROLLER_H
