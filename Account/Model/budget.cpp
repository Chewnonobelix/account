#include "budget.h"

Budget::Budget()
{
	setReference(QDate::currentDate());
	setCategory(Category());
	setId(QUuid());
}

Budget::Budget(const Budget & b): MetaData(b)
{
	m_subs = b.m_subs;
}
Budget::Budget(const QJsonObject& obj): MetaData(obj)
{
	Category c(obj["category"].toObject());
	setCategory(c);

	auto t = obj["targets"].toArray();
	QMap<QDate, Target> tars;
	for(auto it: t)
	{
		Target temp(it.toObject());
		tars[temp.date] = temp;
	}

	setMetadata("targets", tars);
}

Budget::operator QJsonObject() const
{
	auto ret = MetaData::operator QJsonObject();
	ret["category"] = (QJsonObject)category();
	auto t = targets().values();
	QJsonArray array;

	for(auto it: t)
		array<<(QJsonObject)it;

	ret["targets"] = array;

	return ret;
}

Target::operator QJsonObject() const
{
	QJsonObject ret;
	ret["date"] = date.toString("dd-MM-yyyy");
	ret["target"] = target;
	ret["frequency"] = QVariant::fromValue(frequency).toString();

	return ret;
}

Target::Target(QDate d, double t, Account::FrequencyEnum f):
						date(d), target(t), frequency(f)
{

}

Target::Target(const QJsonObject& obj)
{
	target = obj["target"].toDouble();
	date = QDate::fromString(obj["date"].toString(), "dd-MM-yyyy");
	frequency = obj["frequency"].toVariant().value<Account::FrequencyEnum>();
}


QUuid Budget::id() const
{
	return metaData<QUuid>("id");
}

void Budget::setId(QUuid i)
{
	setMetadata("id", i);
}

Account::TypeEnum Budget::type() const
{
	return metaData<Account::TypeEnum>("type");
}

void Budget::setType(Account::TypeEnum type)
{
	setMetadata("type", type);
}

bool Budget::addEntry(Entry e)
{
	auto m_targets = targets();
	if(m_targets.isEmpty())
		return false;

	m_subit = std::find_if(m_subs.begin(), m_subs.end(), [e](const SubBudget& it) {
		return it.in(e.date());
	});

	if(m_subit == m_subs.end())
	{
		createSub(e.date());

		m_subit = std::find_if(m_subs.begin(), m_subs.end(), [e](const SubBudget& it) {
			return it.in(e.date());
		});

	}


	return (*m_subit).addEntry(e);;
}

bool Budget::removeEntry(Entry e)
{
	bool ret = false;

	for(auto it: m_subs)
		if(it.begin() <= e.date() && e.date() <= it.end())
			ret = it.removeEntry(e);

	return ret;
}

bool Budget::updateEntry(Entry e)
{
	bool ret = false;

	for(auto it: m_subs)
		if(it.begin() <= e.date() && e.date() <= it.end())
			ret = it.updateEntry(e);

	return ret;
}

bool Budget::addTarget(QDate d, double t, Account::FrequencyEnum f)
{
	auto m_targets = targets();

	bool ret = !m_targets.contains(d);

	if(ret)
		m_targets[d] = Target {d, t, f};

	setMetadata("targets", m_targets);
	return ret;
}

bool Budget::removeTarget(QDate d)
{
	auto m_targets = targets();

	bool ret = m_targets.remove(d) != 0;
	m_subs.clear();
	setMetadata("targets", m_targets);


	return ret;
}

bool Budget::updateTarget(QDate d, double t)
{
	auto m_targets = targets();
	bool ret = m_targets.contains(d);

	if(ret)
		m_targets[d].target = t;
	setMetadata("targets", m_targets);

	return ret;
}

bool Budget::createSub(QDate d)
{
	if(targets().isEmpty())
		return false;

	Target t;

	if(d <= targets().firstKey())
		t = targets().first();
	else if(d >= targets().lastKey())
		t = targets().last();
	else
	{
		auto targs = targets();
		for(auto it = targs.begin(); it != targs.end(); it++)
			if(d >= it.key() && d < (it+1).key()) t = *it;
	}

	SubBudget sub;
	sub.setBegin(t.date);
	sub.setEnd(t.date.addDays(Account::nbDay(t.date, t.frequency)));
	sub.setReference(t.date);
	sub.setTarget(t.target);

	bool ret = false;
	while (ret = !sub.in(d))
	{
		if(d > sub.begin())
		{
			sub.setBegin(next(sub.begin(), t.frequency));
			sub.setEnd(next(sub.end(), t.frequency));
		}
		else
		{
			sub.setBegin(previous(sub.begin(), t.frequency));
			sub.setEnd(previous(sub.end(), t.frequency));
		}
	}

	m_subs[sub.begin()] = sub;

	return !ret;
}

double Budget::current(QDate d)
{
	double ret = 0;
	for(auto it: m_subs)
		if(it.in(d))
			ret = it.current();
	return ret;
}

Category Budget::category() const
{
	return metaData<Category>("category");
}

void Budget::setCategory(Category c)
{
	setMetadata("category", c);
}

Budget& Budget::operator = (const Budget& b)
{
	MetaData::operator=(b);
	m_subs = b.m_subs;

	return *this;
}

Budget& Budget::operator <<(Entry e)
{
	if (e.category() == category())
		if(!addEntry(e))
			updateEntry(e);

	return *this;
}

Budget& Budget::operator >>(Entry e)
{
	removeEntry(e);
	return *this;
}

QDate Budget::next(QDate d, Account::FrequencyEnum f) const
{
	switch(f)
	{
	case Account::FrequencyEnum::Day:
		return d.addDays(1);
	case Account::FrequencyEnum::Week:
		return d.addDays(8);
	case Account::FrequencyEnum::Month:
		return d.addMonths(1);
	case Account::FrequencyEnum::Quarter:
		return d.addMonths(4);
	case Account::FrequencyEnum::Year:
		return d.addYears(1);
	default:
		return d;
	}
}

QDate Budget::previous(QDate d, Account::FrequencyEnum f) const
{
	switch(f)
	{
	case Account::FrequencyEnum::Day:
		return d.addDays(-1);
	case Account::FrequencyEnum::Week:
		return d.addDays(-8);
	case Account::FrequencyEnum::Month:
		return d.addMonths(-1);
	case Account::FrequencyEnum::Quarter:
		return d.addMonths(-4);
	case Account::FrequencyEnum::Year:
		return d.addYears(-1);
	default:
		return d;
	}
}

QDate Budget::reference() const
{
	return metaData<QDate>("reference");
}

void Budget::setReference(QDate d)
{
	setMetadata("reference", d);
}

QMap<QDate, Target> Budget::targets() const
{
	return metaData<QMap<QDate, Target>>("targets");
}

QMap<QDate, SubBudget> Budget::subs() const
{
	return m_subs;
}
