#include "frequency.h"
#include <QDebug>

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

Frequency::Frequency(): m_nbGroup(0)
{
    m_referenceEntry.setType("outcome");
}

Frequency::Frequency(const Frequency& f): m_id(f.id()), m_freq(f.freq()),
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
    ret.setMetadata("frequency", id());
    ret.setId(-1);
    return ret;
}

QList<LinkedEntry> Frequency::entries() const
{
    return m_entriesId;    
}

Frequency& Frequency::operator<< (const Entry& e)
{   
    if(e.hasMetadata("frequency") && e.metaData<int>("frequency") == id())
    {
        LinkedEntry le;
        le.m_id = e.id();
        le.m_date = e.date();
        le.m_group = e.metaData<int>("freqGroup");
        m_entriesId<<le;
    }
    return *this;
}

bool Frequency::isUnlimited() const
{
    return !m_end.isValid();
}

QString Frequency::name() const
{
    return m_referenceEntry.info().title();
}

QVariantList  Frequency::listEntries() const
{
    QVariantList ret;
    
    for(auto it: entries())
    {
        QVariantMap m;
        m["id"] = QString::number(it.m_id);
        m["date"] = (it.m_date);
        m["group"] = it.m_group;
        m["isVisible"] = true;
        ret<<QVariant::fromValue(m);;
    }
    
    return ret;
}

QVariantList  Frequency::listEntries(int group) const
{
    QVariantList ret;
    
    for(auto it: entries())
    {
        if(it.m_group == group)
        {
            QVariantMap m;
            m["id"] = QString::number(it.m_id);
            m["date"] = (it.m_date);
            m["group"] = it.m_group;
            ret<<QVariant::fromValue(m);
        }
    }
    
    return ret;
}

