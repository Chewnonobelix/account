#include "entry.h"

Entry::Entry()
{
    setId(QUuid());
    setAccount(QString());
    setValue(0);
    setSupport(Account::CB);
    setDate(QDate::currentDate());
    setType("outcome");
}

Entry& Entry::operator = (const Entry& e)
{
    MetaData& md = *this;

    md = e;
    setInfo(e.info());

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

QString Entry::type() const
{
    return metaData<QString>("type");
}

void Entry::setType(QString type)
{
    setMetadata("type", type);
}

Information Entry::info() const
{
    return m_info;
}

void Entry::setInfo(Information info)
{
    m_info = info;
}

bool operator == (const Entry& e1, const Entry& e2)
{
    return e1.id() == e2.id();
}

bool operator < (const Entry& e1, const Entry& e2)
{
    return (e1.date() < e2.date()) ||
            (e1.info() < e2.info());
}

QString Entry::label() const
{
    return info().title();
}

Entry::operator QVariantMap() const
{
    QVariantMap ret;
    ret.insert("id", id());
    ret.insert("account", account());
    ret.insert("value", value());
    ret.insert("date", date());
    ret.insert("type", type());
    ret.insert("info", QVariant::fromValue(info()));
    ret.insert("label", label());
    ret.insert("isBlock", isBlocked());
    ret.insert("support", QVariant::fromValue(support()));

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

Account::EntryTypeEnum Entry::support() const
{
    return hasMetadata("support") ? metaData<Account::EntryTypeEnum>("support") : Account::CB;
}

void Entry::setSupport(Account::EntryTypeEnum s)
{
    setMetadata("support", s);
}
