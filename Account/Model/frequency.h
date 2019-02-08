#ifndef FREQUENCY_H
#define FREQUENCY_H

#include "accountglobal.h"
#include "entry.h"

class Frequency
{
private:
    int m_id;
    AccountEnum::FrequencyEnum m_occurancy;
    Entry m_template;
    QMap<QDate, Entry> m_past;

    int toDayNumber(AccountEnum::FrequencyEnum, QDate) const;

public:
    Frequency();
    Frequency(const Frequency&);
    ~Frequency();

    Frequency& operator=(const Frequency&);

    QMap<QDate,Entry> getNew(QDate) const;
    QMap<QDate, Entry> getPrev(QDate) const;

    AccountEnum::FrequencyEnum occurancy() const;
    void setOccurancy(AccountEnum::FrequencyEnum );
    Entry getTemplate() const;
    void setTemplate(Entry);
    QMap<QDate, Entry> past() const;
    void setPast(QMap<QDate, Entry>);
    int id() const;
    void setId(int id);
};

#endif // FREQUENCY_H
