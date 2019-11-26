#ifndef CONTROLLERCOMMON_H
#define CONTROLLERCOMMON_H

#include <QQmlApplicationEngine>

#include "abstractcontroller.h"

class ControllerCommon: public AbstractController
{
    Q_OBJECT
private:
    QObject* m_view;
    QQmlApplicationEngine m_engine;
    
public:
    ControllerCommon() = default;
    ~ControllerCommon() = default;
    
    int exec();
};

#endif // CONTROLLERCOMMON_H
