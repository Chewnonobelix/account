#include "controllersettings.h"

ControllerSettings::ControllerSettings(): m_settings(QSettings::IniFormat, QSettings::UserScope, "Chewnonobelix Inc", "Account")
{
    qDebug()<<m_settings.allKeys();
    qDebug()<<m_language.load(QLocale(QLocale::system().uiLanguages().last()), "account_en.qm");
    
    qDebug()<<QCoreApplication::installTranslator(&m_language)<<m_language.isEmpty();
    
    if(!m_settings.allKeys().contains("Database/Main"))
        m_settings.setValue("Database/Main", "ControllerDB");
    
    if(!m_settings.allKeys().contains("Database/UseBackup"))
        m_settings.setValue("Database/UseBackup", false);
    
    if(!m_settings.allKeys().contains("Database/Backup"))
        m_settings.setValue("Database/Backup", "ControllerXMLMulti");
    
    if(!m_settings.allKeys().contains("Feature/"+QObject::tr("Frequency")))
        m_settings.setValue("Feature/"+QObject::tr("Frequency"), true);
    
    if(!m_settings.allKeys().contains("Feature/"+QObject::tr("Budget")))
        m_settings.setValue("Feature/"+QObject::tr("Budget"), true);
    
    if(!m_settings.allKeys().contains("Feature/"+QObject::tr("CommonExpanse")))
        m_settings.setValue("Feature/"+QObject::tr("CommonExpanse"), true);
    
    if(!m_settings.allKeys().contains("Language"))
        m_settings.setValue("Language",  "English");
    
    //    m_language.load("account_en.qm");
    //    m_language.load("account_fr.qm");
    
    qDebug()<<QLocale::system();
    QLocale::setDefault(QLocale(QLocale::system().uiLanguages().last()));
    qDebug()<<QLocale(QLocale::system().uiLanguages().first())<<QLocale(QLocale::system().uiLanguages().last());
    qDebug()<<QLocale();
    //    qDebug()<<QLocale::system();
    
    qDebug()<<featuresList();
    
    for(auto it: featuresList())
    {
        qDebug()<<it<<featureEnable(it);
    }
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
