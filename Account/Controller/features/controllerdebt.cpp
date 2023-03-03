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
    connect((QThread *) &debt->m_filler, SIGNAL(finished()), debt.data(), SLOT(endFill()));
    debt->exec();
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
        //        m_filler.entries = db()->selectEntry().values();
        m_filler.start();
    }

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

void ControllerDebt::endFill()
{
    QVariantList list;
    int index = -1;

    for (auto it : m_debts) {
        list << QVariant::fromValue(it);
        if (it.id() == m_currentId)
            index = list.count() - 1;
    }

    emit modelChanged(list, index);
}

void ControllerDebt::addDebt()
{
    Debt d;
    auto i = d.initial();
    i.setBlocked(true);
    d.setInitial(i);
    db()->addDebt(d);
}

void ControllerDebt::onNameChanged(QString id, QString name)
{
//    Debt d = db()->selectDebt()[QUuid::fromString(id)];
//    d.setName(name);
//    auto el = db()->selectEntry();

//    //    Entry e = el[QUuid::fromString(id)];
//    //    e.setTitle(name);
//    //    d.setInitial(e);
//    db()->updateDebt(d);
}

void ControllerDebt::onTimeChanged(QString id, int time)
{
    Debt d = db()->selectDebt()[QUuid::fromString(id)];
    d.setNb(time);
    db()->updateDebt(d);
}

void ControllerDebt::onFreqChanged(QString id, Account::FrequencyEnum freq)
{
    Debt d = db()->selectDebt()[QUuid::fromString(id)];
    d.setFreq(freq);
    db()->updateDebt(d);
}

void ControllerDebt::onRateChanged(QString id, double rate)
{
    Debt d = db()->selectDebt()[QUuid::fromString(id)];
    d.setRate(rate / 100.0);
    db()->updateDebt(d);
}

void ControllerDebt::onRemoved(QString id)
{
    Debt d = db()->selectDebt()[QUuid::fromString(id)];
    db()->removeDebt(d);
}

void ControllerDebt::onInitialDateChanged(QString id, QDate d)
{
//    Debt b = db()->selectDebt()[QUuid::fromString(id)];
//    auto el = db()->selectEntry();

//    //    Entry e = el[QUuid::fromString(id)];

//    //    e.setDate(d);
//    //    b.setInitial(e);
//    db()->updateDebt(b);
}

void ControllerDebt::onInitialTypeChanged(QString id, int t)
{
//    Debt b = db()->selectDebt()[QUuid::fromString(id)];
//    auto el = db()->selectEntry();

//    //    Entry e = el[QUuid::fromString(id)];

//    //    e.setType(Account::TypeEnum(t));
//    //    b.setInitial(e);
//    db()->updateDebt(b);
}

void ControllerDebt::onInitialValueChanged(QString id, double v)
{
//    auto el = db()->selectEntry();

//    //    Entry e = el[QUuid::fromString(id)];

//        Debt b = db()->selectDebt()[QUuid::fromString(id)];

//    //    e.setValue(v);
//    //    b.setInitial(e);

//    db()->updateDebt(b);
}

void ControllerDebt::onInitialCategoryChanged(QString id, QString c)
{
//    Debt b = db()->selectDebt()[QUuid::fromString(id)];
//    auto el = db()->selectEntry();

//    // Entry e = el[QUuid::fromString(id)];
//    // auto cat = db()->selectCategory()[e.type()][QUuid::fromString(c)];
//    // e.setCategory(cat);
//    // b.setInitial(e);
//    db()->updateDebt(b);
}

void ControllerDebt::onInitialSupportChanged(QString id, int s)
{
//    Debt b = db()->selectDebt()[QUuid::fromString(id)];
//    auto el = db()->selectEntry();

////    Entry e = el[QUuid::fromString(id)];

////    e.setSupport((Account::SupportEnum) s);
////    b.setInitial(e);
//    db()->updateDebt(b);
}

void ControllerDebt::onNewCategory(int type, QString cat)
{
    auto cats = db()->selectCategory()[Account::TypeEnum(type)].values();

    auto f = std::find_if(cats.begin(), cats.end(), [cat, type](Category c) {
        return c.name() == cat;
    });

    if (f != cats.end())
        return;

    Category c;
    c.setType(Account::TypeEnum(type));
    c.setName(cat);
    db()->addCategory(c);
}

void ControllerDebt::generate(QString id)
{
    auto debt = m_debts[QUuid::fromString(id)];

    auto ret = debt.generate();

    if (ret) {
        auto list = debt.entries();
        for (auto it : list) {
            it.setTitle(debt.name() + "_" + it.date().toString("dd-MM-yyyy"));

            it.setAccount(currentAccount());
            db()->addEntry(it);
        }

        db()->updateDebt(debt);
        emit db()->s_updateEntry();
    }
}
