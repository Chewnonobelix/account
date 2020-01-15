#include "controllersettings.h"

ControllerSettings::ControllerSettings(): m_settings(QSettings::IniFormat, QSettings::UserScope, "Chewnonobelix Inc", "Account")
{
//    m_settings.setValue("Database/Main", "ControllerDB");
}

QString ControllerSettings::database() const
{
    return m_settings.value("Database/Main").toString();
}

int ControllerSettings::exec()
{
    return 0;
}
