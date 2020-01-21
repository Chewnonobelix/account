#ifndef CONTROLLERFREQUENCY_H
#define CONTROLLERFREQUENCY_H

#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlPropertyMap>
#include <QThread>
#include "abstractcontroller.h"
#include "featurebuilder.h"
#include "Model/frequency.h"
#include "Model/entry.h"

class Worker: public QThread
{
    Q_OBJECT
public:
    QList<Entry> list;
    QString name;

    Worker& operator =(const Worker&);
    Worker(const Worker&) = delete;
    Worker() = default;

public slots:
    void run();

signals:
    void s_finish(QString);
    void s_add(const Entry&);
};

class ControllerFrequency: public AbstractController, public FeatureBuilder
{
    Q_OBJECT 
    
private:
    QMap<int, Frequency> m_freqs;
    QMap<QString, Worker*> m_workers;

    QObject* m_manager, *m_generate;
    QQmlApplicationEngine m_eng;
    
    QList<QVariant> m_model; 
    
    void loadCat();
public:
    ControllerFrequency();
    inline ControllerFrequency(const ControllerFrequency& f): AbstractController(f) {}
    ~ControllerFrequency() = default;
    
    void addEntry(int);
    
    int exec();
    void closeManager();
    
    void setManager(QObject*);

    QSharedPointer<FeatureBuilder> build(QQmlApplicationEngine *, QObject *, QList<AbstractController *>);

signals:
    void s_addEntry(Entry);
    void s_select(int = -1);
    void s_show();

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
};

Q_DECLARE_METATYPE(ControllerFrequency)

#endif // CONTROLLERFREQUENCY_H
