#include "controllersettings.h"

QMap<QString, QSharedPointer<FeatureBuilder>> ControllerSettings::registredFeatures = QMap<QString, QSharedPointer<FeatureBuilder>>();

ControllerSettings::ControllerSettings(): m_settings(QSettings::IniFormat, QSettings::UserScope, "Chewnonobelix Inc", "Account")
{
        for(auto it: m_settings.allKeys())
        qDebug()<<it<<m_settings.value(it).type()<<m_settings.value(it).typeName();
}

ControllerSettings::~ControllerSettings()
{
    for(auto it: m_language)
        delete it;
}

void ControllerSettings::init(QQmlEngine & engine)
{
    QObject* root = ((QQmlApplicationEngine&)engine).rootObjects().first();

    m_view = root->findChild<QObject*>("settings");

    connect(m_view, SIGNAL(accepted()), this, SLOT(save()));

    QDir dir;
    auto list = dir.entryInfoList(QStringList("*.qm"));
    QStringList availableLanguage;
    for(auto it: list)
    {
        m_language[it.baseName()] = new QTranslator();
        m_language[it.baseName()]->load(it.fileName());
        availableLanguage<<it.baseName();
    }

    setLanguage(language());

    QObject* obj = m_view->findChild<QObject*>("language");
    obj->setProperty("model", QVariant::fromValue(availableLanguage));
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
    QCoreApplication::removeTranslator(m_language[this->language()]);
    m_settings.setValue("Language", language);
    QCoreApplication::installTranslator(m_language[language]);
    
    emit s_language();
}

bool ControllerSettings::featureEnable(QString feature) const
{
    return m_settings.value("Feature/"+feature).toBool();
}

void ControllerSettings::setFeatureEnable(QString feature, bool enable)
{
    m_settings.setValue("Feature/"+feature, enable);
    emit s_finish();
}

QStringList ControllerSettings::featuresList() const
{
    QStringList ret = m_settings.allKeys().filter("Feature");
    
    ret.replaceInStrings("Feature/", "");
    
    return ret;
}

void ControllerSettings::open()
{
    QObject* obj = m_view->findChild<QObject*>("language");
    int index;
    QMetaObject::invokeMethod(obj, "indexOfValue", Q_RETURN_ARG(int, index), Q_ARG(QVariant, language()));
    obj->setProperty("currentIndex", index);


    obj = m_view->findChild<QObject*>("budget");
    obj->setProperty("checked", featureEnable("BudgetFeature"));

    obj = m_view->findChild<QObject*>("frequency");
    obj->setProperty("checked", featureEnable("FrequencyFeature"));

    obj = m_view->findChild<QObject*>("common");
    obj->setProperty("checked", featureEnable("CommonExpanseFeature"));

    obj = m_view->findChild<QObject*>("primary")->property("item").value<QObject*>();
    QMetaObject::invokeMethod(obj, "indexOfValue", Q_RETURN_ARG(int, index), Q_ARG(QVariant, database()));
    obj->setProperty("currentIndex", index);

    obj = m_view->findChild<QObject*>("useBackup");
    obj->setProperty("checked", backupEnable());

    if(backupEnable())
    {
        obj = m_view->findChild<QObject*>("backup")->property("item").value<QObject*>();
        QMetaObject::invokeMethod(obj, "indexOfValue", Q_RETURN_ARG(int, index), Q_ARG(QVariant, backup()));
        obj->setProperty("currentIndex", index);
    }

    QMetaObject::invokeMethod(m_view, "open");
}

int ControllerSettings::exec()
{
    return 0;
}

void ControllerSettings::save()
{
    QObject* obj = m_view->findChild<QObject*>("language");

    setLanguage(obj->property("currentText").toString());

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

QString ControllerSettings::sortingRole() const
{
    return m_settings.value("SortingRole", "label").toString();
}

void ControllerSettings::setSortingRole(QString role)
{
    m_settings.setValue("SortingRole", role);
}

Qt::SortOrder ControllerSettings::sortOrder() const
{
    return (Qt::SortOrder)(m_settings.value("SortOrder", 0).toInt());
}

void ControllerSettings::setSortOrdre(Qt::SortOrder order)
{
    m_settings.setValue("SortOrder", order);
}

