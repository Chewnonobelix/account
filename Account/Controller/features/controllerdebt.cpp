#include "controllerdebt.h"

ControllerDebt::ControllerDebt(const ControllerDebt &d) : AbstractController(d) {}

QSharedPointer<FeatureBuilder> ControllerDebt::build(QQmlApplicationEngine *engine, QObject *)
{
    auto debt = QSharedPointer<ControllerDebt>::create();
    auto *context = engine->rootContext();
    context->setContextProperty("_debt", debt.data());

    auto comp = QQmlComponent(engine, QUrl("qrc:/Debt/Debt.qml"));
    debt->view = comp.create();

    connect(db(), &InterfaceDataSave::s_updateDebt, debt.data(), &ControllerDebt::exec);
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
    m_debts = db()->selectDebt();

    if (!m_filler.model)
        m_filler.model = &m_debts;

    if (!m_filler.isRunning()) {
        m_filler.entries = db()->selectEntry(currentAccount()).values();

        m_filler.start();
    }

    QVariantList list;

    for (auto it : m_debts)
        list << QVariant::fromValue(it);

    emit modelChanged(list, -1);
    return 0;
}

void ControllerDebt::addDebt()
{
    Debt d;
    db()->addDebt(d);
}

void ControllerDebt::onNameChanged(QString id, QString name)
{
    Debt d = db()->selectDebt()[QUuid::fromString(id)];
    d.setName(name);
    db()->updateDebt(d);
}
