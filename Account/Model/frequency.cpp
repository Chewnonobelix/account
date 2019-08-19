#include "frequency.h"

int Frequency::id() const
{
    return m_id;
}

void Frequency::setId(int id)
{
    m_id = id;
}

Account::FrequencyEnum Frequency::freq() const
{
    return m_freq;
}

void Frequency::setFreq(Account::FrequencyEnum freq)
{
    m_freq = freq;
}

QDate Frequency::end() const
{
    return m_end;
}

void Frequency::setEnd(QDate end)
{
    m_end = end;
}

Frequency::Frequency()
{
    
}

Entry Frequency::clone(const Entry & e) const
{
    Entry ret(e);
    int t = Account::nbDay(e.date(), freq());
    ret.setDate(e.date().addDays(t));
    ret.setMetadata("frequency", id());

    return ret;
}

QSet<int> Frequency::entries() const
{
    return m_entriesId;    
}

Frequency& Frequency::operator<< (const Entry& e)
{
    
    m_entriesId<<e.id();
    return *this;
}

bool Frequency::isUnlimited() const
{
    return !m_end.isValid();
}
