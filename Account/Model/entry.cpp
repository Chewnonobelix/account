#include "entry.h"

Entry::Entry()
{
    setId(QUuid());
    setAccount(QString());
    setValue(0);
    setSupport(Account::SupportEnum::CB);
    setDate(QDate::currentDate());
    setType(Account::TypeEnum::Outcome);
    setTitle(QString());
    setEstimated(false);
    setCategory(Category());
}

Entry::Entry(const QJsonObject &j) : MetaData(j)
{
 auto c = Category(j["category"].toObject());
 setCategory(c);
}

Entry& Entry::operator = (const Entry& e)
{
    MetaData::operator=(e);

    return *this;
}

QUuid Entry::id() const
{
    return metaData<QUuid>("id");
}

void Entry::setId(QUuid id)
{
    setMetadata("id", id);
}

QString Entry::account() const
{
    return metaData<QString>("account");
}

void Entry::setAccount(QString account)
{
    setMetadata("account", account);
}

double Entry::value() const
{
    return floor(metaData<double>("value") * 100.0) / 100.0;
}

void Entry::setValue(double value)
{
    setMetadata("value", floor(value * 100.0) / 100.0);
}

QDate Entry::date() const
{
    return metaData<QDate>("date");
}

void Entry::setDate(QDate date)
{
    setMetadata("date", date);
}

Account::TypeEnum Entry::type() const
{
    return metaData<Account::TypeEnum>("type");
}

void Entry::setType(Account::TypeEnum type)
{
    setMetadata("type", type);
}

bool operator == (const Entry& e1, const Entry& e2)
{
    return e1.id() == e2.id();
}

bool operator < (const Entry& e1, const Entry& e2)
{
    return (e1.date() < e2.date());
}

Entry::operator QVariantMap() const
{
    QVariantMap ret;
    ret.insert("id", id());
    ret.insert("account", account());
    ret.insert("value", value());
    ret.insert("date", date());
    ret.insert("type", QVariant::fromValue(type()));
    ret.insert("label", title());
    ret.insert("isBlock", isBlocked());
    ret.insert("support", QVariant::fromValue(support()));
    //    ret.insert("category", category());

    return ret;
}

bool Entry::isBlocked() const
{
    return hasMetadata("blocked") && metaData<bool>("blocked");
}

void Entry::setBlocked(bool b)
{
    setMetadata("blocked", b);
}

Account::SupportEnum Entry::support() const
{
    return hasMetadata("support") ? metaData<Account::SupportEnum>("support") : Account::SupportEnum::CB;
}

void Entry::setSupport(Account::SupportEnum s)
{
    setMetadata("support", s);
}

QString Entry::title() const
{
    return metaData<QString>("title");
}

void Entry::setTitle(QString title)
{
    setMetadata("title", title);
}

bool Entry::estimated() const
{
    return metaData<bool>("estimated");
}

void Entry::setEstimated(bool estimated)
{
    setMetadata("estimated", estimated);
}

Category Entry::category() const
{
    return metaData<Category>("category");
}

void Entry::setCategory(Category category)
{
    setMetadata("category", category);
}

Entry::operator QJsonObject() const
{
 auto ret = MetaData::operator QJsonObject();
 ret["category"] = QJsonObject(category());
 return ret;
}

bool Entry::isFrequency() const
{
 return metadataList().contains("frequency");
}

bool Entry::isDebt() const
{
 return metadataList().contains("debt");
}
