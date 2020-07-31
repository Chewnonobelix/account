#include "frequency.h"
#include <QDebug>

QUuid Frequency::id() const
{
    return metaData<QUuid>("id");
}

void Frequency::setId(QUuid id)
{
    setMetadata("id", id);
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

QDate Frequency::begin() const
{
    return m_begin;
}

void Frequency::setBegin(QDate end)
{
    m_begin = end;
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

Frequency::Frequency()
{
    m_referenceEntry.setType("outcome");
}

Frequency& Frequency::operator =(const Frequency& f)
{
    MetaData& m = *this;
    m = f;
    setFreq(f.freq());
    
    m_entriesId.clear();
    for(auto it: f.entries())
        m_entriesId<<it;
    
    setEnd(f.end());
    setBegin(f.begin());
    setReferenceEntry(f.referenceEntry());
    setNbGroup(f.nbGroup());
    setEndless(f.endless());
    
    return *this;
}

Entry Frequency::clone(const Entry & e) const
{
    Entry ret(e);
    ret.setMetadata("frequency", id());
    ret.setMetadata("notemit", true);
    ret.setId(QUuid());
    return ret;
}

QList<QVariant> Frequency::entries() const
{
    return m_entriesId;    
}

int Frequency::count() const
{
    return m_entriesId.count();
}

Frequency& Frequency::operator<< (const Entry& e)
{   
    if(e.hasMetadata("frequency") && e.metaData<QUuid>("frequency") == id())
    {
        if(e.date() < begin())
            setBegin(e.date());
        if(e.date() > end() || m_entriesId.isEmpty())
            setEnd(e.date());
        
        QVariantMap le;
        le["id"] = e.id();
        le["date"] = e.date();
        le["freqGroup"] = e.metaData<int>("freqGroup");
        le["isVisible"] = true;
        m_entriesId<<le;
    }
    return *this;
}

QString Frequency::name() const
{
    return m_referenceEntry.info().title();
}

QVariantList Frequency::listEntries(int group) const
{
    QVariantList ret;
    
    for(auto it: entries())
        if(it.toMap()["freqGroup"].toInt() == group)
            ret<<it;
    
    return ret;
}


bool Frequency::endless() const
{
    return m_endless;
}

void Frequency::setEndless(bool e)
{
    m_endless = e;
}
