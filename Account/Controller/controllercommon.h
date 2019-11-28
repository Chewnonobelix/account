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
    void init();
    
public slots:
    void closeCommon(bool);
    void addCommon(QString);
    void removeCommon(int);
    void addCommonEntry();
    
};

#endif // CONTROLLERCOMMON_H
