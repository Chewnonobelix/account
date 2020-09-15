#ifndef CONTROLLERDEBT_H
#define CONTROLLERDEBT_H

#include "../core/abstractcontroller.h"
#include "Model/debt.h"
#include "featurebuilder.h"
#include <QtCore/qglobal.h>

class ControllerDebt : public AbstractController, public FeatureBuilder
{
    Q_OBJECT

public:
    ControllerDebt() = default;
    ControllerDebt(const ControllerDebt &);
    ~ControllerDebt() = default;

    virtual QSharedPointer<FeatureBuilder> build(QQmlApplicationEngine *, QObject *);
    virtual QString displayText() const;
    virtual QString baseText() const;
    virtual void checker();

    int exec();
};

#endif // CONTROLLERDEBT_H
