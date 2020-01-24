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

void ControllerSettings::init(QQmlEngine & engine)
{
    QObject* root = ((QQmlApplicationEngine&)engine).rootObjects().first();

    m_view = root->findChild<QObject*>("settings");

    connect(m_view, SIGNAL(accepted()), this, SLOT(save()));
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

void ControllerSettings::open()
{
    QMetaObject::invokeMethod(m_view, "open");
}

int ControllerSettings::exec()
{
    return 0;
}

void ControllerSettings::save()
{
    QObject* obj = m_view->findChild<QObject*>("language");

    m_settings.setValue("Language", obj->property("currentText"));

    obj = m_view->findChild<QObject*>("budget");
    setFeatureEnable("BudgetFeature", obj->property("checked").toBool());

    obj = m_view->findChild<QObject*>("frequency");
    setFeatureEnable("FrequencyFeature", obj->property("checked").toBool());

    obj = m_view->findChild<QObject*>("common");
    setFeatureEnable("CommonExpanseFeature", obj->property("checked").toBool());

    obj = m_view->findChild<QObject*>("primary")->property("item").value<QObject*>();
    setDatabase(obj->property("currentValue").toString());

    obj = m_view->findChild<QObject*>("useBackup");
    setBackupEnable(obj->property("checked").toBool());

    if(backupEnable())
    {
        obj = m_view->findChild<QObject*>("backup")->property("item").value<QObject*>();
        setBackup(obj->property("currentValue").toString());
    }
}
