#include "frequency.h"

Frequency::Frequency(): m_id(-1), m_occurancy(AccountEnum::never)
{

}

Frequency::Frequency(const Frequency & f): m_id(f.id()), m_occurancy(f.occurancy()),
    m_template(f.getTemplate()), m_past(f.past())
{

}

Frequency::~Frequency()
{}

Frequency& Frequency::operator = (const Frequency& f)
{
    setId(f.id());
    setPast(f.past());
    setTemplate(f.getTemplate());
    setOccurancy(f.occurancy());

    return *this;
}
AccountEnum::FrequencyEnum Frequency::occurancy() const
{
    return m_occurancy;
}

void Frequency::setOccurancy(AccountEnum::FrequencyEnum occurancy)
{
    m_occurancy = occurancy;
}

Entry Frequency::getTemplate() const
{
    return m_template;
}

void Frequency::setTemplate(Entry t)
{
    m_template = t;
}

QMap<QDate, Entry> Frequency::past() const
{
    return m_past;
}

void Frequency::setPast(QMap<QDate, Entry> past)
{
    m_past = past;
}

QMap<QDate,Entry> Frequency::getNew(QDate today) const
{
    QMap<QDate, Entry> ret;

    auto days = m_past.keys();
    std::sort(days.begin(), days.end());

    QDate last = days.last();

    while(last < today)
    {
        Entry e = getTemplate();
        e.setId(-1);
        Information inf = e.info();
        inf.setId(-1);
        inf.setEstimated(last > QDate::currentDate());
        e.setInfo(inf);
        e.setDate(last.addDays(toDayNumber(occurancy(), last)));

        ret[e.date()] = e;
    }

    return ret;
}

QMap<QDate, Entry> Frequency::getPrev(QDate lastDay) const
{
    return getNew(lastDay);
}

int Frequency::id() const
{
    return m_id;
}

void Frequency::setId(int id)
{
    m_id = id;
}

int Frequency::toDayNumber(AccountEnum::FrequencyEnum fe, QDate d1) const
{
    QDate d2;
    int ret;
    switch(fe)
    {
    case AccountEnum::FrequencyEnum::daily:
        ret = 1;
        break;
    case AccountEnum::FrequencyEnum::weekly:
        ret = 7;
        break;
    case AccountEnum::FrequencyEnum::bimonthly:
        ret = 14;
        break;
    case AccountEnum::FrequencyEnum::monthly:
        d2 = d1.addMonths(1);
        ret = qAbs(d1.daysTo(d2));
        break;
    case AccountEnum::FrequencyEnum::annualy:
        d2 = d1.addYears(1);
        ret = qAbs(d1.daysTo(d2));
        break;

    default:
        ret = 0;
    }

    return ret;
}


