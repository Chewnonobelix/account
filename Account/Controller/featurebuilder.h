#ifndef FEATUREBUILDER_H
#define FEATUREBUILDER_H

#include <QSharedPointer>
#include <QQmlApplicationEngine>
#include "abstractcontroller.h"

struct FeatureBuilder
{
    virtual QSharedPointer<FeatureBuilder> build(QQmlApplicationEngine*, QObject*, QList<AbstractController*>) = 0;    
    virtual QString displayText() const = 0;
    virtual QString baseText() const = 0;
    QObject* view = nullptr;
    virtual ~FeatureBuilder() = default;
};

#endif // FEATUREBUILDER_H
