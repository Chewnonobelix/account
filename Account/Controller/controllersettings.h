#ifndef CONTROLLERSETTINGS_H
#define CONTROLLERSETTINGS_H

#include <QSettings>
#include <QTranslator>
#include <QCoreApplication>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlContext>
#include "abstractcontroller.h"
#include "featurebuilder.h"

class ControllerSettings: public AbstractController
{
    Q_OBJECT
    
private:
    QSettings m_settings;
    QTranslator m_language;
    
    static QMap<QString, QSharedPointer<FeatureBuilder>> registredFeatures;
    
    QObject* m_view;

public:
    ControllerSettings();

    void init(QQmlEngine&);
    QString database() const;
    void setDatabase(QString);
    QString backup() const;
    void setBackup(QString);
    bool backupEnable() const;
    void setBackupEnable(bool);
    
    QString language() const;
    void setLanguage(QString);

    bool featureEnable(QString) const;
    void setFeatureEnable(QString, bool);
    QStringList featuresList() const;
    
    static void registerFeature(QSharedPointer<FeatureBuilder>);
    static inline QStringList registredFeature() {return registredFeatures.keys();}
    int exec();

public slots:
    void open();
};

#endif // CONTROLLERSETTINGS_H
