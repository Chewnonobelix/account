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
    return metaData<Account::FrequencyEnum>("freq");
}

void Frequency::setFreq(Account::FrequencyEnum freq)
{
    setMetadata("freq", freq);
}

QDate Frequency::end() const
{
    return metaData<QDate>("end");
}

void Frequency::setEnd(QDate end)
{
    setMetadata("end", end);
}

QDate Frequency::begin() const
{
    return metaData<QDate>("begin");
}

void Frequency::setBegin(QDate end)
{
    setMetadata("begin", end);
}

Entry Frequency::referenceEntry() const
{
 return metaData<Entry>("reference");
}

void Frequency::setReferenceEntry(Entry referenceEntry)
{
 setMetadata("reference", referenceEntry);
}

int Frequency::nbGroup() const
{
    return metaData<int>("nbGroup");
}

void Frequency::setNbGroup(int nbGroup)
{
    setMetadata("nbGroup", nbGroup);
}

Frequency::Frequency()
{
 //    m_referenceEntry.setType(Account::Outcome);
 setId(QUuid());
 setFreq(Account::FrequencyEnum::Unique);
 setBegin(QDate::currentDate());
 setEnd(QDate::currentDate());
 setNbGroup(0);
 setEndless(false);
}

Frequency::Frequency(const QJsonObject &obj) : MetaData(obj)
{
 Entry ref(obj["reference"].toObject());
 setReferenceEntry(ref);
}

Frequency::operator QJsonObject() const
{
 auto ret = MetaData::operator QJsonObject();
 ret["reference"] = QJsonObject(referenceEntry());
 return ret;
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
 if (e.hasMetadata("frequency") && e.metaData<QUuid>("frequency") == id()) {
  if (e.date() < begin())
   setBegin(e.date());
  if (e.date() > end() || m_entriesId.isEmpty())
   setEnd(e.date());

  QVariantMap le;
  le["id"] = e.id().toString();
  le["date"] = e.date();
  le["freqGroup"] = e.metaData<int>("freqGroup");
  le["isVisible"] = true;
  m_entriesId << le;
 }
 return *this;
}

QString Frequency::name() const
{
 return referenceEntry().title();
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
    return metaData<bool>("endless");
}

void Frequency::setEndless(bool e)
{
    setMetadata("endless", e);
}
