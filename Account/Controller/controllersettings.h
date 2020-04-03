#ifndef CONTROLLERSETTINGS_H
#define CONTROLLERSETTINGS_H

#include <QSettings>
#include <QTranslator>
#include <QCoreApplication>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlContext>
#include <QDir>
#include <QFileInfo>
#include "abstractcontroller.h"
#include "featurebuilder.h"
#include "xmltosql.h"

class ControllerSettings: public AbstractController
{
    Q_OBJECT
    
private:
    QSettings m_settings;
    QMap<QString, QTranslator*> m_language;
    
    static QMap<QString, QSharedPointer<FeatureBuilder>> registredFeatures;
    
    QObject* m_view;
    InterfaceDataSave* back = nullptr;

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

    QString language() const;
    void setLanguage(QString);

    bool featureEnable(QString) const;
    void setFeatureEnable(QString, bool);
    QStringList featuresList() const;

    QString sortingRole() const;
    void setSortingRole(QString);
    Qt::SortOrder sortOrder() const;
    void setSortOrdre(Qt::SortOrder);


    static void registerFeature(QSharedPointer<FeatureBuilder>);
    static inline QStringList registredFeature() {return registredFeatures.keys();}
    static inline QSharedPointer<FeatureBuilder> features(QString name) {return registredFeatures[name];}
    int exec();

signals:
    void s_finish();
    void s_language();
    void s_closedb();

public slots:
    void open();
    void save();
    void restore();
    void backup();
};

#endif // CONTROLLERSETTINGS_H
