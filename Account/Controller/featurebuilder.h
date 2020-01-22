#ifndef FEATUREBUILDER_H
#define FEATUREBUILDER_H

#include <QSharedPointer>
#include <QQmlEngine>
#include "abstractcontroller.h"

class FeatureBuilder
{
public:
    virtual QSharedPointer<FeatureBuilder> build(QQmlApplicationEngine*, QObject*, QList<AbstractController*>) = 0;    
    virtual QString displayText() const = 0;
};

#endif // FEATUREBUILDER_H
