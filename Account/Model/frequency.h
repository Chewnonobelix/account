#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <QSet>
#include <QJSValue>
#include "accountglobal.h"
#include "entry.h"

struct LinkedEntry{
    int m_id;
    QDate m_date;
    int m_group;       
};

Q_DECLARE_METATYPE(LinkedEntry)

class Frequency
{
    Q_GADGET
    
    Q_PROPERTY(int id READ id)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(Entry reference READ referenceEntry)
    Q_PROPERTY(QDate end READ end)
    Q_PROPERTY(int nbGroup READ nbGroup)
    Q_PROPERTY(Account::FrequencyEnum freq READ freq)
    
public:
    
private:
    
    int m_id;
    Account::FrequencyEnum m_freq;
    QList<LinkedEntry> m_entriesId;
    QDate m_end;
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
    
    QList<LinkedEntry> entries() const;
    Q_INVOKABLE QVariantList listEntries() const;
    Q_INVOKABLE QVariantList listEntries(int) const;
    
    Frequency& operator<< (const Entry&);
    Frequency& operator<< (int);
    
    Entry clone(const Entry&) const;
    QDate end() const;
    void setEnd(QDate);
    Q_INVOKABLE bool isUnlimited() const;
    Entry referenceEntry() const;
    void setReferenceEntry(Entry referenceEntry);
    int nbGroup() const;
    void setNbGroup(int nbGroup);
    QString name() const;
};

Q_DECLARE_METATYPE(Frequency)

#endif // FREQUENCY_H
