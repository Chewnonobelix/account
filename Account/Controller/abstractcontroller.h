#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QString>
#include <QDate>
#include <QMultiMap>
#include <QObject>
#include <QDebug>
#include <QRandomGenerator>
#include <QThread>
#include <QMutex>
#include "Model/entry.h"
#include "Model/total.h"

#include "interfacedatasave.h"

class AbstractController: public QObject
{
    Q_OBJECT

    class CalcThread: public QThread
    {
    private:
    protected:
        void run();

    public:
        CalcThread(int, QList<Entry>, Total*);
        int index;
        QList<Entry> l;
        Total* ret;
        static QSharedPointer<QMutex> mut;
        static QSet<int> indexes;
    };

private:
    static QString m_account;
    static Total m_accountTotal;
    static QList<QSharedPointer<CalcThread>> pool;

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

public slots:
    void calculTotal();

signals:
    void s_totalChanged();

};

#endif // ABSTRACTCONTROLLER_H
