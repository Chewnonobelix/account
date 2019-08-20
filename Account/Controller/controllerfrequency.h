#ifndef CONTROLLERFREQUENCY_H
#define CONTROLLERFREQUENCY_H

#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>

#include "abstractcontroller.h"
#include "Model/frequency.h"
#include "Model/entry.h"

class ControllerFrequency: public AbstractController
{
private:
    QMap<int, Frequency> m_freqs;
    QObject* m_manager, *m_generate;
    QQmlApplicationEngine m_eng;
    
public:
    ControllerFrequency();
    
    void addEntry(int);
    
    int exec();

public slots:
    void generate(QDate, QDate);
    void openGenerate(int);
    
};

#endif // CONTROLLERFREQUENCY_H
