#ifndef CONTROLLERFREQUENCY_H
#define CONTROLLERFREQUENCY_H

#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlPropertyMap>
#include <QThread>
#include "abstractcontroller.h"
#include "Model/frequency.h"
#include "Model/entry.h"

class Worker: public QObject
{
    Q_OBJECT
public:
    QList<Entry> list;
    QString name;

    Worker& operator =(const Worker&);
    Worker(const Worker&);
    Worker() = default;

public slots:
    void run();

signals:
    void s_finish(QString);
    void s_add(QSharedPointer<Entry>);
};

class ControllerFrequency: public AbstractController
{
    Q_OBJECT 
private:
    QMap<int, Frequency> m_freqs;
//    QMap<QString, Worker*> m_threads;
//    Worker m_thread;
    QThread m_thread;
    QObject* m_manager, *m_generate;
    QQmlApplicationEngine m_eng;
    
    QList<QVariant> m_model; 
    
    void loadCat();
public:
    ControllerFrequency();
    ~ControllerFrequency() = default;
    
    void addEntry(int);
    
    int exec();
    void closeManager();

signals:
    void s_addEntry(Entry);
    
public slots:
    void endThread(QString);
    void generate(QString, QString);
    void openGenerate(int);
    
    void openManager();
    
    void addFrequency();
    void removeFrequency(int);
    
    void addNewCategory(QString);
    
    void updateFreqName(int, QString);
    void updateFreqValue(int, double);
    void updateFreqCat(int, QString);
    void updateFreqType(int, QString);
    void updateFreqFreq(int, int);
    
    void displayEntry(int);

    void addFEntry(QSharedPointer<Entry>);
};

#endif // CONTROLLERFREQUENCY_H
