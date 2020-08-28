#ifndef CONTROLLERSETTINGS_H
#define CONTROLLERSETTINGS_H

#include "../data/xmltosql.h"
#include "../features/controllerbudget.h"
#include "../features/controllercommon.h"
#include "../features/controllerfrequency.h"
#include "../features/featurebuilder.h"
#include "abstractcontroller.h"
#include "controllersynchronization.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickItem>
#include <QSettings>
#include <QTranslator>

class ControllerSettings: public AbstractController
{
    Q_OBJECT
    
private:
    QSettings m_settings;
    QMap<QString, QTranslator*> m_language;
    
    static QMap<QString, QSharedPointer<FeatureBuilder>> registredFeatures;
    
    QObject* m_view = nullptr, *m_splash = nullptr;
    InterfaceDataSave* back = nullptr;
    TransfertDatabase m_backupper;
    ControllerSynchronization m_synchro;

public:
    ControllerSettings();
    virtual ~ControllerSettings();

    void init(QQmlEngine &);

    Q_INVOKABLE QString database() const;
    Q_INVOKABLE void setDatabase(QString);
    Q_INVOKABLE QString backupType() const;
    Q_INVOKABLE void setBackup(QString);
    Q_INVOKABLE bool backupEnable() const;
    Q_INVOKABLE void setBackupEnable(bool);
    Q_INVOKABLE bool autoBackup() const;
    Q_INVOKABLE void setAutobackup(bool);
    Q_INVOKABLE inline bool isBackuping() const { return m_backupper.isRunning(); }
    Q_INVOKABLE QString language() const;
    Q_INVOKABLE void setLanguage(QString);
    Q_INVOKABLE QString languageCode() const;

    Q_INVOKABLE bool featureEnable(QString) const;
    Q_INVOKABLE void setFeatureEnable(QString, bool);
    Q_INVOKABLE QStringList featuresList() const;

    Q_INVOKABLE QString sortingRole() const;
    Q_INVOKABLE void setSortingRole(QString);
    Q_INVOKABLE Qt::SortOrder sortOrder() const;
    Q_INVOKABLE void setSortOrdre(Qt::SortOrder);
    Q_INVOKABLE QString currentProfile() const;
    Q_INVOKABLE void setCurrentProfile(QString);
    Q_INVOKABLE QString currentAccount() const;
    Q_INVOKABLE void setCurrentAccount(QString);

    Q_INVOKABLE bool syncServer() const;
    Q_INVOKABLE void setSyncServer(bool);

    InterfaceDataSave* createDb(QString, bool = false) const;

    static void registerFeature(QSharedPointer<FeatureBuilder>);
    static inline QStringList registredFeature() {return registredFeatures.keys();}
    static inline QSharedPointer<FeatureBuilder> features(QString name) {return registredFeatures[name];}
    int exec();

signals:
    void s_finish();
    void s_language();
    void s_closedb();
    void s_finishBackup();

public slots:
    void open();
    void save();
    void restore(QString back);
    void backup();
    void endBackup();
    void endRestore();
};

#endif // CONTROLLERSETTINGS_H
