#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <QSet>
#include <QJSValue>
#include "accountglobal.h"
#include "entry.h"

class ACCOUNT_EXPORT Frequency
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
    Q_PROPERTY(bool endless READ endless)
    
public:
    
private:
    
    int m_id = -1;
    Account::FrequencyEnum m_freq = Account::FrequencyEnum::Unique;
    QList<QVariant> m_entriesId;
    QDate m_end = QDate::currentDate();
    QDate m_begin = QDate::currentDate();
    Entry m_referenceEntry;
    int m_nbGroup = 0;
    bool m_endless = false;

public:
    Frequency();
    Frequency(const Frequency&) = default;
    ~Frequency() = default;
    
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
    bool endless() const;
    void setEndless(bool);
    
    Entry referenceEntry() const;
    void setReferenceEntry(Entry referenceEntry);
    int nbGroup() const;
    void setNbGroup(int nbGroup);
    QString name() const;
    int count() const;
};

Q_DECLARE_METATYPE(Frequency)

#endif // FREQUENCY_H
