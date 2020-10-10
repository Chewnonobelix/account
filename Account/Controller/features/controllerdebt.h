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

    Q_PROPERTY(QString currentId READ id WRITE setId)

private:
    Filler<QUuid, Debt> m_filler;
    QMap<QUuid, Debt> m_debts;
    QUuid m_currentId;

    void setId(QString);
    QString id() const;

public:
    ControllerDebt() = default;
    ControllerDebt(const ControllerDebt &);
    ~ControllerDebt() = default;

    virtual QSharedPointer<FeatureBuilder> build(QQmlApplicationEngine *, QObject *);
    virtual QString displayText() const;
    virtual QString baseText() const;
    virtual void checker();

    Q_INVOKABLE int exec();
    Q_INVOKABLE void generate(QString);

signals:
    void modelChanged(QVariantList, int);

public slots:
    void addDebt();
    void onRemoved(QString);
    void onNameChanged(QString, QString);
    void onTimeChanged(QString, int);
    void onFreqChanged(QString, Account::FrequencyEnum);
    void onRateChanged(QString, double);

    void onInitialDateChanged(QString, QDate);
    void onInitialTypeChanged(QString, int);
    void onInitialValueChanged(QString, double);
    void onInitialCategoryChanged(QString, QString);
    void onInitialSupportChanged(QString, int);
	void onNewCategory(int, QString);

	void endFill();
};

#endif // CONTROLLERDEBT_H
