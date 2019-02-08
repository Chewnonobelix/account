#ifndef FREQUENCY_H
#define FREQUENCY_H

#include "accountglobal.h"
#include "entry.h"

class Frequency
{
private:
    int m_id;
    AccountEnum::FrequencyEnum m_when;
    Entry m_template;
    QMap<QDate, Entry> m_past;

public:
    Frequency();
};

#endif // FREQUENCY_H
