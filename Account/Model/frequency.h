#ifndef FREQUENCY_H
#define FREQUENCY_H

#include "accountglobal.h"
#include "entry.h"

class Frequency
{
private:
    int m_id;
    Account::FrequencyEnum m_freq;
    
public:
    Frequency();
    
    int id() const;
    void setId(int);
    
    Account::FrequencyEnum freq() const;
    void setFreq(Account::FrequencyEnum);
    
    Entry clone(const Entry&) const;
};

#endif // FREQUENCY_H
