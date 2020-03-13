#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <QSet>
#include <QJSValue>
#include "accountglobal.h"
#include "entry.h"

class Frequency
{
    Q_GADGET
    
    Q_PROPERTY(int id READ id)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(Entry reference READ referenceEntry)
    Q_PROPERTY(QDate end READ end)
    Q_PROPERTY(QDate begin READ begin)
    Q_PROPERTY(int nbGroup READ nbGroup)
    Q_PROPERTY(Account::FrequencyEnum freq READ freq)
    Q_PROPERTY(int count READ count)
    
public:
    
private:
    
    int m_id;
    Account::FrequencyEnum m_freq;
    QList<QVariant> m_entriesId;
    QDate m_end;
    QDate m_begin;
    Entry m_referenceEntry;
    int m_nbGroup;
    

public:
    Frequency();
    Frequency(const Frequency&);
    ~Frequency();
    
    Frequency& operator =(const Frequency&);
    
    int id() const;
    void setId(int);
    
    Account::FrequencyEnum freq() const;
    void setFreq(Account::FrequencyEnum);
    
    Q_INVOKABLE QVariantList  entries() const;
    Q_INVOKABLE QVariantList listEntries(int) const;
    
    Frequency& operator<< (const Entry&);
    
    Entry clone(const Entry&) const;
    QDate end() const;
    QDate begin() const;
    void setEnd(QDate);
    void setBegin(QDate);
    Q_INVOKABLE bool isUnlimited() const;
    Entry referenceEntry() const;
    void setReferenceEntry(Entry referenceEntry);
    int nbGroup() const;
    void setNbGroup(int nbGroup);
    QString name() const;
    int count() const;
};

Q_DECLARE_METATYPE(Frequency)

#endif // FREQUENCY_H
