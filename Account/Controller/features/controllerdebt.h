#ifndef CONTROLLERDEBT_H
#define CONTROLLERDEBT_H

#include "../core/abstractcontroller.h"
#include "Model/debt.h"
#include "featurebuilder.h"
#include "filler.h"
#include <QtCore/qglobal.h>

class ControllerDebt : public AbstractController, public FeatureBuilder
{
    Q_OBJECT

private:
    Filler<QUuid, Debt> m_filler;
    QMap<QUuid, Debt> m_debts;

public:
    ControllerDebt() = default;
    ControllerDebt(const ControllerDebt &);
    ~ControllerDebt() = default;

    virtual QSharedPointer<FeatureBuilder> build(QQmlApplicationEngine *, QObject *);
    virtual QString displayText() const;
    virtual QString baseText() const;
    virtual void checker();

    Q_INVOKABLE int exec();

signals:
    void modelChanged(QVariantList, int);

public slots:
    void addDebt();
    void onNameChanged(QString, QString);
};

#endif // CONTROLLERDEBT_H
