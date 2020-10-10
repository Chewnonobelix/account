#include "controllerbudget.h"

ControllerBudget::ControllerBudget(const ControllerBudget &b) : AbstractController(b) {}

ControllerBudget::~ControllerBudget()
{
    m_filler.terminate();
}

void ControllerBudget::closeManager()
{
    emit close();
}

void ControllerBudget::openManager()
{
    emit clearCat();
    emit blocked(true);

    //    auto incomes = m_db->selectCategory().values("income");
    //    auto outcomes = m_db->selectCategory().values("outcome");

    auto func = [&](QString type, QStringList list) {
        for (auto it : list) {
            QVariantMap map;
            map.insert("type", type);
            map.insert("catName", it);
            map.insert("has", m_budgets.contains(it));

            emit addCat(QVariant::fromValue(map));
        }
    };

    //    func("income", incomes);
    //    func("outcome", outcomes);

    if (!m_selected.isEmpty())
        emit selectCat(m_selected);

    emit blocked(false);
}

bool ControllerBudget::removeFrom(QUuid id)
{
    bool ret = false;
    Entry e = entry(id);

    //    if (m_budgets.contains(e.category()))
    //        ret = m_budgets[e.category()].removeEntry(e);

    return ret;
}

int ControllerBudget::exec()
{
    if (!m_filler.model)
        m_filler.model = &m_budgets;

    reload();
    openManager();
    return 0;
}

void ControllerBudget::show(QDate date)
{
    QList<QPair<QString,SubBudget>> list;
    m_currentDate = date;
    for (auto it : m_budgets) {
        for (auto it2 : it.subs()) {
            if (it2.in(date))
                list << qMakePair(it.category(), it2);
        }
    }

    emit clearDate();
    QVariantList listret;

    for (int i = 0; i < list.size(); i++) {
        QVariantMap map;
        map.insert("target", list[i].second.target());
        map.insert("currentValue", list[i].second.current());
        map.insert("name", list[i].first);

        listret << map;
    }
    emit dateChanged(QVariant::fromValue(listret));
}

void ControllerBudget::reload()
{
    if(m_filler.isRunning())
        return;

    m_budgets.clear();

    auto l = m_db->selectBudgets();

    for (auto b : l)
        m_budgets[b.category()] = b;

    m_filler.entries = m_db->selectEntry().values();
    m_filler.start();

    show(QDate::currentDate());
}

void ControllerBudget::addTarget(QString cat)
{
    editBudget(cat);
}

void ControllerBudget::removeTarget(QString cat, QString date)
{
    if (m_budgets.contains(cat))
        if(m_budgets[cat].removeTarget(QDate::fromString(date, "dd-MM-yyyy")))
        {
            m_db->updateBudget(m_budgets[cat]);

            reload();
            getTarget(cat);

            showTarget(cat, "", true);
        }

    editBudget(cat);
}

void ControllerBudget::addBudget(QString name)
{
    if(m_budgets.contains(name))
    {
        m_db->removeBudget(m_budgets[name]);        
        m_budgets.remove(name);
    }
    else
    {
        Budget b;
        b.setCategory(name);
        m_budgets[name] = b;
        m_db->addBudget(b);
    }
    m_selected = name;
    
    reload();
    openManager();
}

void ControllerBudget::editBudget(QString cat)
{
    //    if(!m_referenceView)
    //    {
    //        m_referenceView = m_view->findChild<QObject*>("reference");
    //    }

    emit catChanged(cat);
}

void ControllerBudget::editReference(QVariant ref)
{
    QVariantMap map = ref.toMap();

    m_budgets[map["cat"].toString()].setFrequency(QDate::fromString(map["date"].toString(),
                                                                    "dd-MM-yyyy"),
                                                  map["role"].value<Account::FrequencyEnum>());

    m_budgets[map["cat"].toString()].addTarget(QDate::fromString(map["date"].toString(),
                                                                 "dd-MM-yyyy"),
                                               map["value"].toDouble());
    m_db->updateBudget(m_budgets[map["cat"].toString()]);
    reload();
    getTarget(map["cat"].toString());
}

void ControllerBudget::getTarget(QString catName)
{
    auto list = m_budgets[catName].targets();

    emit clearTarget();

    for (auto it = list.begin(); it != list.end(); it++) {
        QVariantMap map;
        map.insert("date", it.key());
        map.insert("target", it.value());
        map.insert("frequency", (int) m_budgets[catName].frequency(it.key()));

        emit addTarget2(map);
    }

    showTarget(catName, "", true);
}

void ControllerBudget::updateEntry(QUuid id)
{
    Entry e = entry(id);

    //    if (m_budgets.contains(e.category())) {
    //        m_budgets[e.category()] >> e;

    //        m_budgets[e.category()] << e;
    //    }

    reload();
}

void ControllerBudget::changeEntry(QString old, QUuid id)
{
    Entry e = entry(id);

    if (m_budgets.contains(old))
        m_budgets[old] >> e;

    //    if (m_budgets.contains(e.category()))
    //        m_budgets[e.category()] << e;

    reload();
}

void ControllerBudget::showTarget(QString catName, QString date, bool all)
{
    QList<SubBudget> list2 ;
    emit clearSub();

    auto list = m_budgets[catName].subs();

    QDate d = QDate::fromString(date, "dd-MM-yyyy");

    for (auto it : list) {
        if (it.reference() == d || all)
            list2 << it;
    }

    for (auto it : list2) {
        QVariantMap map;
        map["begin"] = it.begin();
        map["end"] = it.end();
        map["current"] = it.current();
        map["target"] = it.target();
        map["cat"] = catName;
        emit addSub(map);
    }
}

void ControllerBudget::calDateChange(QVariantList list)
{
    if (list.isEmpty())
        show(QDate::currentDate());
    else
        show(QDate::fromString(list.first().toString(), "dd-MM-yyyy"));
}

QSharedPointer<FeatureBuilder> ControllerBudget::build(QQmlApplicationEngine * engine, QObject * root)
{
    Q_UNUSED(root)
    auto budget = QSharedPointer<ControllerBudget>::create();
    auto* context = engine->rootContext();
    context->setContextProperty("_budget", budget.data());
    QQmlComponent budgetComp(engine, QUrl("qrc:/Budget/BudgetManager.qml"));
    budget->view = budgetComp.create();
    qDebug() << budgetComp.errorString();

    budget->show(QDate::currentDate());
    budget->exec();

    connect(m_db, &InterfaceDataSave::s_updateEntry, budget.data(), &ControllerBudget::updateEntry);
    connect(m_db, &InterfaceDataSave::s_updateCategory, budget.data(), &ControllerBudget::exec);
    budget.dynamicCast<ControllerBudget>()->calDateChange(QVariantList());

    return budget;
}

QString ControllerBudget::displayText() const
{
    return tr("Budget");
}

QString ControllerBudget::baseText() const
{
    return "BudgetFeature";
}
