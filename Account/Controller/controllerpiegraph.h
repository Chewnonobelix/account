#ifndef CONTROLLERPIEGRAPH_H
#define CONTROLLERPIEGRAPH_H

#include <QDebug>
#include <QQmlApplicationEngine>
#include "abstractcontroller.h"

class ControllerPieGraph: public AbstractController
{
    Q_OBJECT
private:
    QObject* m_income = nullptr, *m_outcome = nullptr, *m_view;

    Account::Granularity m_gran = Account::Month;

public:
    ControllerPieGraph(QObject* = nullptr);
    
    void init(const QQmlApplicationEngine&);
    
public slots:
    int exec();    
    void change(int);
    void increment(int = 0);
};

#endif // CONTROLLERPIEGRAPH_H
