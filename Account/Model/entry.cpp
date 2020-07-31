#include "entry.h"

Entry::Entry()
{
    setMetadata("id", QUuid());
}

Entry::Entry(const Entry& e): MetaData(e),
    m_account(e.account()), m_value(e.value()), m_date(e.date()),
    m_type(e.type()), m_info(e.info())
{

}

Entry::~Entry()
{

}

Entry& Entry::operator = (const Entry& e)
{
    MetaData& md = *this;

    md = e;
    setAccount(e.account());
    setValue(e.value());
    setDate(e.date());
    setType(e.type());
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
    return m_account;
}

void Entry::setAccount(QString account)
{
    m_account = account;
}

double Entry::value() const
{
    return floor (m_value * 100.0) / 100.0 ;
}

void Entry::setValue(double value)
{
    m_value = floor (value * 100.0) / 100.0 ;
}

QDate Entry::date() const
{
    return m_date;
}

void Entry::setDate(QDate date)
{
    m_date = date;
}

QString Entry::type() const
{
    return m_type;
}

void Entry::setType(QString type)
{
    m_type = type;
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
