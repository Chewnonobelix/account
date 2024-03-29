#include "entry.h"

Entry::Entry(): m_id(-1)
{
    
}

Entry::Entry(const Entry& e):
    m_id(e.id()), m_account(e.account()), m_value(e.value()), m_date(e.date()),
    m_type(e.type()), m_info(e.info())
{

}

Entry::~Entry()
{

}

Entry& Entry::operator = (const Entry& e)
{
    setId(e.id());
    setAccount(e.account());
    setValue(e.value());
    setDate(e.date());
    setType(e.type());
    setInfo(e.info());

    return *this;
}


int Entry::id() const
{
    return m_id;
}

void Entry::setId(int id)
{
    m_id = id;
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
