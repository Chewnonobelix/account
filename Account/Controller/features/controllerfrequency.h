#ifndef CONTROLLERFREQUENCY_H
#define CONTROLLERFREQUENCY_H

#include "../core/abstractcontroller.h"
#include "Model/entry.h"
#include "Model/frequency.h"
#include "featurebuilder.h"
#include "filler.h"
#include <QMetaMethod>
#include <QMetaObject>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlPropertyMap>
#include <QQuickItem>
#include <QThread>
#include <QQmlContext>

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
    void s_add(Entry&);
    void s_progressChanged(double);
};

class ControllerFrequency: public AbstractController, public FeatureBuilder
{
    Q_OBJECT
    Q_PROPERTY(QUuid currentId MEMBER m_currentId)

private:
    QUuid m_currentId;

    QMap<QUuid, Frequency> m_freqs;
    QMap<QString, Worker*> m_workers;

    QObject *m_manager;
    QQmlApplicationEngine m_eng;

    QList<QVariant> m_model;

    Filler<QUuid, Frequency> m_filler;
    
public:
    ControllerFrequency();
    inline ControllerFrequency(const ControllerFrequency& f): AbstractController(f) {}
    ~ControllerFrequency() = default;
        
    void closeManager();
    
    QSharedPointer<FeatureBuilder> build(QQmlApplicationEngine *, QObject *);
    QString displayText() const;
    QString baseText() const;
    void checker();
				void setQuickView(QList<QString>) override {}

    Q_INVOKABLE QObject* worker(QString) const;
    
signals:
    void s_addEntry(Entry);
    void s_select(int = -1);
    void s_show();

    void displayLink(QVariant);
    void close();
    void catList(QStringList, QStringList);
    void setGenerate(QVariant, int);
    void displayWorker(QVariant);
    void setModel(QVariant, int);

public slots:
    int exec();

    void endThread(QString);
    void generate(QString, QString, QVariant, int);
    void openGenerate(QVariant);
    
    void openManager();
    
    void addFrequency();
	void removeFrequency(QVariant);

	void addNewCategory(QVariant, QString, int);

	void updateFreqName(QVariant, QString);
	void updateFreqValue(QVariant, double);
    void updateFreqCat(QVariant, QString);
    void updateFreqType(QVariant, int);
    void updateFreqFreq(QVariant, int);
    void updateFreqEndless(QVariant, bool);
    void updateFreqSupport(QVariant, int);

    void displayEntry(QVariant);
    void setWorker(QString);

    void endFill();
};

Q_DECLARE_METATYPE(ControllerFrequency)

#endif // CONTROLLERFREQUENCY_H
