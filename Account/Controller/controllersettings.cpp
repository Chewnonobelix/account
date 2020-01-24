#include "controllersettings.h"

QMap<QString, QSharedPointer<FeatureBuilder>> ControllerSettings::registredFeatures = QMap<QString, QSharedPointer<FeatureBuilder>>();

ControllerSettings::ControllerSettings(): m_settings(QSettings::IniFormat, QSettings::UserScope, "Chewnonobelix Inc", "Account")
{
    
    qDebug()<<m_language.load("Francais.qm");
    QCoreApplication::installTranslator(&m_language);
    
    qDebug()<<QLocale::system().uiLanguages();
    
    for(auto it: m_settings.allKeys())
        qDebug()<<it<<m_settings.value(it).type()<<m_settings.value(it).typeName();
}

void ControllerSettings::registerFeature(QSharedPointer<FeatureBuilder> f)
{
    registredFeatures[f->baseText()] = f;
}

QString ControllerSettings::database() const
{
    return m_settings.value("Database/Main").toString();
}

void ControllerSettings::setDatabase(QString db)
{
    m_settings.setValue("Database/Main", db);
}

QString ControllerSettings::backup() const
{
    return m_settings.value("Database/Backup").toString();
}

void ControllerSettings::setBackup(QString back)
{
    m_settings.setValue("Database/Backup", back);
}

bool ControllerSettings::backupEnable() const
{
    return m_settings.value("Database/UseBackup").toBool();
}

void ControllerSettings::setBackupEnable(bool enable)
{
    m_settings.setValue("Database/UseBackup", enable); 
}

QString ControllerSettings::language() const
{
    return m_settings.value("Language").toString();
}

void ControllerSettings::setLanguage(QString language)
{
    m_settings.setValue("Language", language);    
}

bool ControllerSettings::featureEnable(QString feature) const
{
    return m_settings.value("Feature/"+feature).toBool();
}

void ControllerSettings::setFeatureEnable(QString feature, bool enable)
{
    m_settings.setValue("Feature/"+feature, enable);
}

QStringList ControllerSettings::featuresList() const
{
    QStringList ret = m_settings.allKeys().filter("Feature");
    
    ret.replaceInStrings("Feature/", "");
    
    return ret;
}

int ControllerSettings::exec()
{
    return 0;
}
