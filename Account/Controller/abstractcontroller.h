#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QString>
#include <QDate>
#include <QMultiMap>
#include <QObject>

#include "Model/entry.h"
#include "Model/categories.h"
#include "Model/total.h"

class AbstractController: public QObject
{
    Q_OBJECT

private:
    static QStringList m_listAccount;
    static QString m_account;
    static QMultiMap<QDate, Entry> m_entry;

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

};

#endif // ABSTRACTCONTROLLER_H
