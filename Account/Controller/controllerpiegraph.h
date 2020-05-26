#ifndef CONTROLLERPIEGRAPH_H
#define CONTROLLERPIEGRAPH_H

#include <QDebug>
#include <QQmlApplicationEngine>
#include "abstractcontroller.h"

class ControllerPieGraph: public AbstractController
{
    Q_OBJECT
private:
    QObject* m_income = nullptr, *m_outcome = nullptr;
    
public:
    ControllerPieGraph(QObject* = nullptr);
    
    int exec();
    void init(const QQmlApplicationEngine&);
};

#endif // CONTROLLERPIEGRAPH_H
