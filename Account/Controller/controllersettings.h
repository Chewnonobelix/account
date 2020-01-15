#ifndef CONTROLLERSETTINGS_H
#define CONTROLLERSETTINGS_H

#include <QSettings>
#include <QTranslator>
#include "abstractcontroller.h"

class ControllerSettings: public AbstractController
{
    Q_OBJECT
    
private:
    QSettings m_settings;
    QTranslator m_language;
    
public:
    ControllerSettings();
    
    QString database() const;
    
    int exec();
};

#endif // CONTROLLERSETTINGS_H
