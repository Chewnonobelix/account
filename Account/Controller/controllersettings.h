#ifndef CONTROLLERSETTINGS_H
#define CONTROLLERSETTINGS_H

#include "abstractcontroller.h"
#include "controllersynchronization.h"
#include "featurebuilder.h"
#include "xmltosql.h"
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

    void init(QQmlEngine&);
    QString database() const;
    void setDatabase(QString);
    QString backupType() const;
    void setBackup(QString);
    bool backupEnable() const;
    void setBackupEnable(bool);
    bool autoBackup() const;
    void setAutobackup(bool);
    inline bool isBackuping() const {return m_backupper.isRunning();}
    QString language() const;
    void setLanguage(QString);
    QString languageCode() const;

    bool featureEnable(QString) const;
    void setFeatureEnable(QString, bool);
    QStringList featuresList() const;

    QString sortingRole() const;
    void setSortingRole(QString);
    Qt::SortOrder sortOrder() const;
    void setSortOrdre(Qt::SortOrder);
    QString currentProfile() const;
    void setCurrentProfile(QString);
    QString currentAccount() const;
    void setCurrentAccount(QString);

    bool syncServer() const;
    void setSyncServer(bool);

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
