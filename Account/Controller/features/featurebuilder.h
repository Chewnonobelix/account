#ifndef FEATUREBUILDER_H
#define FEATUREBUILDER_H

#include "../core/abstractcontroller.h"
#include <QQmlApplicationEngine>
#include <QSharedPointer>

struct FeatureBuilder
{
    virtual QSharedPointer<FeatureBuilder> build(QQmlApplicationEngine*, QObject*, QList<AbstractController*>) = 0;    
    virtual QString displayText() const = 0;
    virtual QString baseText() const = 0;
    virtual void checker() = 0;
    QObject* view = nullptr;
    virtual ~FeatureBuilder() = default;
};

#endif // FEATUREBUILDER_H
