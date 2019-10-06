#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <QSet>
#include "accountglobal.h"
#include "entry.h"

class Frequency
{
    Q_GADGET
    
    Q_PROPERTY(int id READ id)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(Entry reference READ referenceEntry)
    Q_PROPERTY(QDate end READ end)
    Q_PROPERTY(int nbGroup READ nbGroup)
    Q_PROPERTY(QList<int> entries READ listEntries)
    
private:
    int m_id;
    Account::FrequencyEnum m_freq;
    QSet<int> m_entriesId;
    QDate m_end;
    Entry m_referenceEntry;
    int m_nbGroup;
    
    QList<int> listEntries() const;
    
public:
    Frequency();
    Frequency(const Frequency&);
    ~Frequency();
    
    Frequency& operator =(const Frequency&);
    
    int id() const;
    void setId(int);
    
    Account::FrequencyEnum freq() const;
    void setFreq(Account::FrequencyEnum);
    
    QSet<int> entries() const;
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
