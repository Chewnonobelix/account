#include "entry.h"

Entry::Entry(): m_frequency(-1)
{
    setMetadata("id", -1);
}

Entry::Entry(const Entry& e):
    m_account(e.account()), m_value(e.value()), m_date(e.date()),
    m_type(e.type()), m_info(e.info()), m_metaData(e.m_metaData), m_frequency(e.frequency())
{

}

Entry::~Entry()
{

}

Entry& Entry::operator = (const Entry& e)
{
    setAccount(e.account());
    setValue(e.value());
    setDate(e.date());
    setType(e.type());
    setInfo(e.info());
    setFrequency(e.frequency());
    m_metaData = e.m_metaData;

    return *this;
}


int Entry::id() const
{
    return metaData<int>("id");
}

void Entry::setId(int id)
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

void Entry::setFrequency(int f)
{
    m_frequency = f;
}

int Entry::frequency() const
{
    return m_frequency;
}

bool Entry::hasMetadata() const
{
    return !m_metaData.isEmpty();
}

bool Entry::hasMetadata(QString name) const
{
    return m_metaData.contains(name);
}

QStringList Entry::metaDataList() const
{
    return m_metaData.keys();
}
