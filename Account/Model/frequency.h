#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <QSet>
#include "accountglobal.h"
#include "entry.h"

class Frequency
{
private:
    int m_id;
    Account::FrequencyEnum m_freq;
    QSet<int> m_entriesId;
    QDate m_end;
    int m_referenceEntry;
    
public:
    Frequency();
    
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
};

#endif // FREQUENCY_H