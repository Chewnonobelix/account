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

Entry Frequency::referenceEntry() const
{
    return m_referenceEntry;
}

void Frequency::setReferenceEntry(Entry referenceEntry)
{
    m_referenceEntry = referenceEntry;
}

int Frequency::nbGroup() const
{
    return m_nbGroup;
}

void Frequency::setNbGroup(int nbGroup)
{
    m_nbGroup = nbGroup;
}

Frequency::Frequency(): QObject(nullptr)
{
    
}

Frequency::Frequency(const Frequency& f): QObject(nullptr), m_id(f.id()), m_freq(f.freq()),
    m_entriesId(f.entries()), m_end(f.end()), m_referenceEntry(f.referenceEntry()),
    m_nbGroup(f.nbGroup())
{}

Frequency::~Frequency()
{}

Frequency& Frequency::operator =(const Frequency& f)
{
    setId(f.id());
    setFreq(f.freq());
    
    m_entriesId.clear();
    for(auto it: f.entries())
        m_entriesId<<it;
    
    setEnd(f.end());
    setReferenceEntry(f.referenceEntry());
    setNbGroup(f.nbGroup());
    
    return *this;
}

Entry Frequency::clone(const Entry & e) const
{
    Entry ret(e);
    int t = Account::nbDay(e.date(), freq());
    ret.setDate(e.date().addDays(t));
    ret.setMetadata("frequency", id());
    ret.setId(-1);
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

QString Frequency::name() const
{
    return m_referenceEntry.info().category();
}
