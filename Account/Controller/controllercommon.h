#ifndef CONTROLLERCOMMON_H
#define CONTROLLERCOMMON_H

#include <QQmlApplicationEngine>
#include <QQuickItem>
#include "abstractcontroller.h"
#include "featurebuilder.h"

class ControllerCommon: public AbstractController, public FeatureBuilder
{
    Q_OBJECT
    
private:
    
public:
    QObject* m_view = nullptr;
    
    ControllerCommon() = default;
    inline ControllerCommon(const ControllerCommon & c): AbstractController(c) {}
    ~ControllerCommon() = default;
    
    int exec();
    void init();
    
    QSharedPointer<FeatureBuilder> build(QQmlApplicationEngine *, QObject *, QList<AbstractController *>);
    QString displayText() const;
    QString baseText() const;
    
public slots:
    void closeCommon(bool);
    void addCommon(QString);
    void removeCommon(int);
    void addCommonEntry();
    void removeCommonEntry();
    
};

Q_DECLARE_METATYPE(ControllerCommon)


#endif // CONTROLLERCOMMON_H
