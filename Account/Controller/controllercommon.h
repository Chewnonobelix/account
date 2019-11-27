#ifndef CONTROLLERCOMMON_H
#define CONTROLLERCOMMON_H

#include <QQmlApplicationEngine>

#include "abstractcontroller.h"

class ControllerCommon: public AbstractController
{
    Q_OBJECT
private:
    QQmlApplicationEngine m_engine;
    
public:
    QObject* m_view;
    
    ControllerCommon() = default;
    ~ControllerCommon() = default;
    
    int exec();
    
public slots:
    void closeCommon(bool);
    
};

#endif // CONTROLLERCOMMON_H
