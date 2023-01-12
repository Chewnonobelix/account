#include "controllersettings.h"

QMap<QString, QSharedPointer<FeatureBuilder>> ControllerSettings::registredFeatures = QMap<QString, QSharedPointer<FeatureBuilder>>();

ControllerSettings::ControllerSettings(): m_settings(QSettings::IniFormat, QSettings::UserScope, "Chewnonobelix Inc", "Account")
{
    qDebug() << "ControllerBudget" << qRegisterMetaType<ControllerBudget>("BudgetFeature");
    qDebug() << "ControllerFrequency" << qRegisterMetaType<ControllerFrequency>("FrequencyFeature");
    qDebug() << "ControllerCommon" << qRegisterMetaType<ControllerCommon>("CommonExpanseFeature");
    qDebug() << "ControllerDebt" << qRegisterMetaType<ControllerDebt>("DebtFeature");

    for (auto it : {"BudgetFeature", "FrequencyFeature", "CommonExpanseFeature", "DebtFeature"})
        if (!featuresList().contains(it))
            setFeatureEnable(it, false);
}

ControllerSettings::~ControllerSettings()
{
    for(auto it: m_language)
        delete it;
    
    registredFeatures.clear();    
}

void ControllerSettings::init(QQmlEngine & engine)
{
    QQmlComponent syncomp(&engine, "qrc:/Core/Syncing.qml");
    m_splash = syncomp.create();

    QDir dir;
    auto list = dir.entryInfoList(QStringList("*.qm"));
    QStringList availableLanguage;
    for(auto& it: list)
    {
        m_language[it.baseName()] = new QTranslator();
        m_language[it.baseName()]->load(it.fileName());
        availableLanguage<<it.baseName();
    }

    setLanguage(language());

    emit languageChanged(availableLanguage);
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

QString ControllerSettings::backupType() const
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

QString ControllerSettings::languageCode() const
{
    QString ret = m_language[language()]->language();
    return ret.split("_").first();
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
    emit openSettings();
}

int ControllerSettings::exec()
{
    return 0;
}

void ControllerSettings::save()
{
    AbstractController::setDb(database());
}

QString ControllerSettings::sortingRole() const
{
    return m_settings.value("State/SortingRole", "label").toString();
}

void ControllerSettings::setSortingRole(QString role)
{
    m_settings.setValue("State/SortingRole", role);
}

Qt::SortOrder ControllerSettings::sortOrder() const
{
    return (Qt::SortOrder)(m_settings.value("State/SortOrder", 0).toInt());
}

void ControllerSettings::setSortOrdre(Qt::SortOrder order)
{
    m_settings.setValue("State/SortOrder", order);
}

bool ControllerSettings::autoBackup() const
{
    return m_settings.value("Database/autobackup", false).toBool();
}

void ControllerSettings::setAutobackup(bool autobackup)
{
    m_settings.setValue("Database/autobackup", autobackup);
}

void ControllerSettings::restore(QString backdb)
{
    qDebug() << "Restore Backup" << backdb << backdb[backdb.size() - 1];

    emit visibleChanged(true);
    emit backupChanged(false);
    QString dbtype; QChar c = (backdb[backdb.size()-1]);

    switch (c.toLatin1())
    {
    case 's':
        dbtype = "ControllerDB";
        break;
    case 'x':
    default:
        dbtype = "ControllerXMLMulti";
        break;
    }

    QProcess unzipper;

    unzipper.start("7z", QStringList()<<"x"<<backdb.remove("file:///"));

    unzipper.waitForFinished();

//    AbstractController::deleteDb();

    setDb(database());

    auto back = createDb(dbtype, true);
    connect(&m_backupper, &QThread::finished, this, &ControllerSettings::endRestore);

    m_backupper.m_db = back;
    m_backupper.m_backup = m_db;
    m_backupper.start();
}

void ControllerSettings::backup()
{
    if(!backupEnable())
    {
        qDebug()<<"Backup unable";
        return;
    }

    auto back = createDb(backupType(), true);
    if(back && backupEnable())
    {
        emit visibleChanged(true);
        emit backupChanged(true);

        connect(&m_backupper, &TransfertDatabase::finished, this, &ControllerSettings::endBackup);
        m_backupper.m_db = m_db;
        m_backupper.m_backup = back;

        m_backupper.start();
        qDebug()<<thread()<<m_backupper.thread();

    }
}

InterfaceDataSave* ControllerSettings::createDb(QString type, bool b) const
{
    InterfaceDataSave* ret = nullptr;

    auto t = QMetaType::fromName(type.toLatin1());
    if(!t.isRegistered())
        throw QString("Unknow DB type");

    ret = (InterfaceDataSave*)(t.create());
    ret->setBackup(b);
    ret->init();

    return ret;
}

void ControllerSettings::endBackup()
{
    disconnect(&m_backupper, &QThread::finished, this, &ControllerSettings::endBackup);
    delete m_backupper.m_backup;
    m_backupper.m_backup = m_backupper.m_db = nullptr;
    if(m_backupper.isSucess())
    {
        QProcess zipper;
        QString date = QDateTime::currentDateTime().toString("dd_MM_yyyy_hh_mm_ss");
        QStringList argument;
        argument<<"-sdel"<<"-r"<<"a"<<"backup_"+date+".bck"+(backupType() == "ControllerXMLMulti" ? "x" : "s")<< (backupType() == "ControllerXMLMulti" ? "data_backup/" : "account_backup");
        zipper.start("7z", argument);
        zipper.waitForFinished();
        qDebug()<<zipper.readAllStandardOutput();
    }
    qDebug() << "Backup sucess" << m_backupper.isSucess();

    emit visibleChanged(false);

    emit s_finishBackup();
}

void ControllerSettings::endRestore()
{
    disconnect(&m_backupper, &QThread::finished, this, &ControllerSettings::endRestore);

    qDebug()<<"Restore"<<m_backupper.isSucess();
    emit visibleChanged(false);

    delete m_backupper.m_db;
    m_backupper.m_backup = m_backupper.m_db = nullptr;
    QDir dir;
    auto filters = dir.entryInfoList(QStringList({"data_backup", "account_backup"}));

    qDebug()<<filters.size();
    for(auto it: filters)
    {
        qDebug()<<it<<it.filePath();
        if(it.isDir())
        {
            QDir d;
            d.cd(it.filePath());
            d.removeRecursively();
        }
        else
        {
            QFile::remove(it.filePath());
        }
    }

    emit s_finishBackup();
}

QString ControllerSettings::currentProfile() const
{
    return m_settings.value("State/Profile", "Default").toString();
}

void ControllerSettings::setCurrentProfile(QString profile)
{
    m_settings.setValue("State/Profile", profile);
}

QString ControllerSettings::currentAccount() const
{
    return m_settings.value("State/Account", "").toString();

}
void ControllerSettings::setCurrentAccount(QString account)
{
    m_settings.setValue("State/Account", account);
}

bool ControllerSettings::syncServer() const
{
    return m_settings.value("Sync/serverEnable", false).toBool();
}

void ControllerSettings::setSyncServer(bool s)
{
    m_settings.setValue("Sync/serverEnable", s);
    //    m_synchro.openServer(s);
}
