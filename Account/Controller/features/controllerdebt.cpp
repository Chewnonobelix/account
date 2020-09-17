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
    int index = -1;
    for (auto it : m_debts) {
        list << QVariant::fromValue(it);
        if (it.id() == m_currentId)
            index = list.count() - 1;
    }

    emit modelChanged(list, index);
    return 0;
}

void ControllerDebt::setId(QString id)
{
    m_currentId = QUuid::fromString(id);
}

QString ControllerDebt::id() const
{
    return m_currentId.toString();
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
