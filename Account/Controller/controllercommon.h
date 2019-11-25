#ifndef CONTROLLERCOMMON_H
#define CONTROLLERCOMMON_H

#include "abstractcontroller.h"

class ControllerCommon: public AbstractController
{
    Q_OBJECT
private:
    QObject* m_view;
    
public:
    ControllerCommon() = default;
    ~ControllerCommon() = default;
    
    int exec();
};

#endif // CONTROLLERCOMMON_H
