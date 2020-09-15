#include "controllerdebt.h"

ControllerDebt::ControllerDebt(const ControllerDebt &d) : AbstractController(d) {}

QSharedPointer<FeatureBuilder> ControllerDebt::build(QQmlApplicationEngine *engine, QObject *)
{
    auto debt = QSharedPointer<ControllerDebt>::create();
    auto *context = engine->rootContext();
    context->setContextProperty("_debt", debt.data());

    auto comp = QQmlComponent(engine, QUrl("qrc:/Debt/Debt.qml"));
    debt->view = comp.create();

    return debt;
}

QString ControllerDebt::displayText() const
{
    return tr("Debt");
}

QString ControllerDebt::baseText() const
{
    return "DebtFeature";
}

void ControllerDebt::checker() {}

int ControllerDebt::exec()
{
    return 0;
}
