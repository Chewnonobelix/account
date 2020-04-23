#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QString>
#include <QDate>
#include <QMultiMap>
#include <QObject>
#include <QDebug>
#include <QRandomGenerator>
#include <QThread>
#include "Model/entry.h"
//#include "Model/categories.h"
#include "Model/total.h"

#include "interfacedatasave.h"

class AbstractController: public QObject
{
    Q_OBJECT
    
private:
    static QString m_account;
    static Total m_accountTotal;
    
protected:
    static InterfaceDataSave* m_db;
    static QThread* m_dbThread;

public:
    AbstractController();
    inline AbstractController(const AbstractController&): QObject(nullptr) {}
    
    ~AbstractController();
    
    virtual int exec() = 0;
    
    static void setCurrentAccount(QString);
    static QString currentAccount();
    
    static void addEntry(const Entry&);
    static void updateEntry(const Entry&);
    
    static Entry entry(int);
    
    static void setDb(QString);
    static void deleteDb();
    
    static Total accountTotal();
    static void calculTotal();
};

#endif // ABSTRACTCONTROLLER_H
