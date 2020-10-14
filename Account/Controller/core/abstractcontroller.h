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

#include "../data/interfacedatasave.h"

class AbstractController: public QObject
{
    Q_OBJECT

    class CalcThread: public QThread
    {
    private:
    protected:
        void run();

    public:
        CalcThread(int, QList<Entry>, QMap<QDate, Total>*);
        int index;
        QList<Entry> l;
        QMap<QDate, Total>* ret;
        static QSharedPointer<QMutex> mut;
        static QSet<int> indexes;
        static int nbRunning;
    };

private:
    static QString m_account;
    static QMap<QDate, Total> m_accountTotal;
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
    
    static void addEntry(Entry&);
    static void updateEntry(Entry &);
    
    static Entry entry(QUuid);

    static InterfaceDataSave *db();
    static void setDb(QString);
    static void deleteDb();
    
    static Total accountTotal();
    static QMap<QDate, Total> allTotal();

public slots:
    void calculTotal();
    void finishTotalThread();

signals:
    void s_totalChanged(int, int);
};

#endif // ABSTRACTCONTROLLER_H
