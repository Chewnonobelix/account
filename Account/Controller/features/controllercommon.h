#ifndef CONTROLLERCOMMON_H
#define CONTROLLERCOMMON_H

#include "../core/abstractcontroller.h"
#include "featurebuilder.h"
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQmlContext>
class ControllerCommon: public AbstractController, public FeatureBuilder
{
    Q_OBJECT
    
private:
    
public:
    QObject* m_view = nullptr;
    
    ControllerCommon() = default;
    inline ControllerCommon(const ControllerCommon & c): AbstractController(c) {}
    ~ControllerCommon() = default;
    
    void init();
    
    QSharedPointer<FeatureBuilder> build(QQmlApplicationEngine *, QObject *);
    QString displayText() const;
    QString baseText() const;
    void checker() {}

public slots:
    void closeCommon(QString, bool);
    void addCommon(QString);
    void removeCommon(QVariant);
    void addCommonEntry();
    void removeCommonEntry(QString, QString, QString);
    int exec();

signals:
    void commonModelChanged(QVariantList);
    void currentIndexChanged(int);
};

Q_DECLARE_METATYPE(ControllerCommon)


#endif // CONTROLLERCOMMON_H
