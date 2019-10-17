#ifndef CONTROLLERFREQUENCY_H
#define CONTROLLERFREQUENCY_H

#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlPropertyMap>
#include "abstractcontroller.h"
#include "Model/frequency.h"
#include "Model/entry.h"

class ControllerFrequency: public AbstractController
{
    Q_OBJECT 
private:
    QMap<int, Frequency> m_freqs;
    
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

public slots:
    void generate(QDate, QDate);
    void openGenerate(int);
    
    void openManager();
    
    void addFrequency();
    void removeFrequency(int);
    
    void addNewCategory(QString);
    
    void updateFreqName(int, QString);
    void updateFreqValue(int, double);
    void updateFreqCat(int, QString);
    void updateFreqType(int, QString);
};

#endif // CONTROLLERFREQUENCY_H
