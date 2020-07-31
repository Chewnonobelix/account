#ifndef CONTROLLERFREQUENCY_H
#define CONTROLLERFREQUENCY_H

#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlPropertyMap>
#include <QMetaMethod>
#include <QMetaObject>
#include <QThread>
#include "abstractcontroller.h"
#include "featurebuilder.h"
#include "Model/frequency.h"
#include "Model/entry.h"
#include "filler.h"

class Worker: public QThread
{
    Q_OBJECT

    Q_PROPERTY(double progress READ progress NOTIFY s_progressChanged)
    
private:
    double m_progress;

public:
    QList<Entry> list;
    QString name;

    Worker& operator =(const Worker&);
    Worker(const Worker&) = delete;
    Worker() = default;

    double progress() const;

public slots:
    void run();

signals:
    void s_finish(QString);
    void s_add(const Entry&);
    void s_progressChanged(double);
};

class ControllerFrequency: public AbstractController, public FeatureBuilder
{
    Q_OBJECT 
    
private:
    QMap<QUuid, Frequency> m_freqs;
    QMap<QString, Worker*> m_workers;

    QObject* m_manager, *m_generate;
    QQmlApplicationEngine m_eng;
    
    QList<QVariant> m_model; 
    
    void loadCat();
    Filler<QUuid, Frequency> m_filler;
    
public:
    ControllerFrequency();
    inline ControllerFrequency(const ControllerFrequency& f): AbstractController(f) {}
    ~ControllerFrequency() = default;
        
    void closeManager();
    
    void setManager(QObject*);

    QSharedPointer<FeatureBuilder> build(QQmlApplicationEngine *, QObject *, QList<AbstractController *>);
    QString displayText() const;
    QString baseText() const;
    void checker();

    Q_INVOKABLE QObject* worker(QString) const;
    
signals:
    void s_addEntry(Entry);
    void s_select(int = -1);
    void s_show();

public slots:
    int exec();

    void endThread(QString);
    void generate(QString, QString);
    void openGenerate(QUuid);
    
    void openManager();
    
    void addFrequency();
    void removeFrequency(QUuid);
    
    void addNewCategory(QString);
    
    void updateFreqName(QUuid, QString);
    void updateFreqValue(QUuid, double);
    void updateFreqCat(QUuid, QString);
    void updateFreqType(QUuid, QString);
    void updateFreqFreq(QUuid, int);
    void updateFreqEndless(QUuid, bool);
    void updateFreqSupport(QUuid, int);

    void displayEntry(QUuid);
    void setWorker(QString);

    void endFill();
};

Q_DECLARE_METATYPE(ControllerFrequency)

#endif // CONTROLLERFREQUENCY_H
