#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <QSet>
#include "accountglobal.h"
#include "entry.h"

class Frequency: public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(int id READ id)
    Q_PROPERTY(QString name READ name)
    
private:
    int m_id;
    Account::FrequencyEnum m_freq;
    QSet<int> m_entriesId;
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
    
    QSet<int> entries() const;
    Frequency& operator<< (const Entry&);
    
    Entry clone(const Entry&) const;
    QDate end() const;
    void setEnd(QDate);
    bool isUnlimited() const;
    Entry referenceEntry() const;
    void setReferenceEntry(Entry referenceEntry);
    int nbGroup() const;
    void setNbGroup(int nbGroup);
    QString name() const;
};

Q_DECLARE_METATYPE(Frequency)

#endif // FREQUENCY_H
