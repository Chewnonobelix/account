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

	auto incomes = m_db->selectCategory()[Account::TypeEnum::Income];
	auto outcomes = m_db->selectCategory()[Account::TypeEnum::Outcome];

	auto func = [&](Account::TypeEnum type, auto list) {
		for (auto it : list.values()) {
			QVariantMap map;
			map.insert("type", type == Account::TypeEnum::Income ? tr("Income") : tr("Outcome"));
			map.insert("catName", it.name());
			map.insert("id", it.id());

			bool has = std::find_if(m_budgets.begin(), m_budgets.end(), [it, type](Budget b) {
															return b.category().name() == it.name() && (it.both() || it.type() == type);
														}) != m_budgets.end();

			map.insert("has", has);

			emit addCat(QVariant::fromValue(map));
		}
	};

	func(Account::TypeEnum::Income, incomes);
	func(Account::TypeEnum::Outcome, outcomes);

	if (!m_selected.isEmpty())
		emit selectCat(m_selected);

	emit blocked(false);
}

bool ControllerBudget::removeFrom(QUuid id)
{
	bool ret = false;
	Entry e = entry(id);

	if (m_budgets.contains(e.metaData<QUuid>("budget")))
		ret = m_budgets[e.metaData<QUuid>("budget")].removeEntry(e);

	return ret;
}

int ControllerBudget::exec()
{
	if (!m_filler.model)
	{
		m_filler.model = &m_budgets;
		connect(&m_filler, QThread::finished, this, &ControllerBudget::onEndFill);
		connect(db(), &InterfaceDataSave::s_updateBudget, this, &ControllerBudget::exec);
	}

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
				list << qMakePair(it.category().name(), it2);
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

	m_budgets = m_db->selectBudgets();


	m_filler.entries = m_db->selectEntry().values();
	m_filler.start();

	show(QDate::currentDate());
}

QVariant ControllerBudget::get(QString id) const
{
	if(QUuid::fromString(id).isNull())
		return QVariant();

	return QVariant::fromValue(m_budgets[QUuid::fromString(id)]);
}

void ControllerBudget::addTarget(QString cat)
{
	editBudget(cat);
}

void ControllerBudget::removeTarget(QString cat, QString date)
{
	//TODO
	if (m_budgets.contains(cat))
		if(m_budgets[cat].removeTarget(QDate::fromString(date, "dd-MM-yyyy")))
		{
			m_db->updateBudget(m_budgets[cat]);

			reload();

			showTarget(cat, "", true);
		}

	editBudget(cat);
}

void ControllerBudget::addBudget(QString id, int type)
{
	Budget b;
	auto c = db()->selectCategory()[Account::TypeEnum(type)][QUuid::fromString(id)];
	b.setCategory(c);
	b.setType(Account::TypeEnum(type));
	m_db->addBudget(b);
	auto tn = QMetaEnum::fromType<Account::TypeEnum>().valueToKey(type);
	c.setMetadata(tn, b.id());
	db()->updateCategory(c);
}

void ControllerBudget::removeBudget(QString id)
{
	auto b = m_budgets[QUuid::fromString(id)];
	auto c = db()->selectCategory()[b.type()][b.category().id()];
	c.setMetadata(QMetaEnum::fromType<Account::TypeEnum>().valueToKey(int(b.type())), QUuid());
	db()->updateCategory(c);
	db()->removeBudget(b);
}

void ControllerBudget::editBudget(QString cat)
{
	emit catChanged(cat);
}

void ControllerBudget::editReference(QVariant ref)
{
	QVariantMap map = ref.toMap();

	auto b = db()->selectBudgets()[map["cat"].toUuid()];
	b.addTarget(QDate::fromString(map["date"].toString(), "dd-MM-yyyy"),
													map["value"].toDouble(), Account::FrequencyEnum(map["role"].toInt()));

	db()->updateBudget(b);

	reload();
	//TODO
	//    m_budgets[map["cat"].toString()].setFrequency(QDate::fromString(map["date"].toString(),
	//                                                  "dd-MM-yyyy"),
	//            map["role"].value<Account::FrequencyEnum>());

	//    m_budgets[map["cat"].toString()].addTarget(QDate::fromString(map["date"].toString(),
	//                                               "dd-MM-yyyy"),
	//            map["value"].toDouble());
	//    m_db->updateBudget(m_budgets[map["cat"].toString()]);
	//    reload();
	//    getTarget(map["cat"].toString());
}

void ControllerBudget::updateEntry(QUuid id)
{
	Entry e = entry(id);

	if (m_budgets.contains(e.metaData<QUuid>("budget"))) {
		m_budgets[e.metaData<QUuid>("budget")] >> e;

		m_budgets[e.metaData<QUuid>("budget")] << e;
	}

	reload();
}

void ControllerBudget::changeEntry(QString old, QUuid id)
{
	Entry e = entry(id);

	if (m_budgets.contains(old))
		m_budgets[old] >> e;

	if (m_budgets.contains(e.metaData<QUuid>("budget")))
		m_budgets[e.metaData<QUuid>("budget")] << e;

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

void ControllerBudget::setQuickView(QList<QString> list)
{
	QList<QDate> ld;
	for(auto it: list)
		ld<<QDate::fromString(it, "dd-MM-yyyy");

	m_quickModel.setDateList(ld);
}

QSharedPointer<FeatureBuilder> ControllerBudget::build(QQmlApplicationEngine * engine, QObject * root)
{
	Q_UNUSED(root)
	auto budget = QSharedPointer<ControllerBudget>::create();
	auto* context = engine->rootContext();
	context->setContextProperty("_budget", budget.data());
	context->setContextProperty("_budgetQuickModel", &budget->m_quickModel);
	QQmlComponent budgetComp(engine, QUrl("qrc:/Budget/BudgetManager.qml"));
	budget->view = budgetComp.create();
	qDebug() << budgetComp.errorString();

	budget->show(QDate::currentDate());
	budget->exec();

	connect(m_db, &InterfaceDataSave::s_updateEntry, budget.data(), &ControllerBudget::updateEntry);
	connect(m_db, &InterfaceDataSave::s_updateCategory, budget.data(), &ControllerBudget::exec);
	connect(m_db, &InterfaceDataSave::s_updateBudget, budget.data(), &ControllerBudget::exec);

	budget->setQuickView(QList<QString>());

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

void ControllerBudget::onEndFill()
{
	m_quickModel.setBudget(m_budgets);
}
