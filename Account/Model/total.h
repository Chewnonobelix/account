#ifndef TOTAL_H
#define TOTAL_H

#include <QDate>
#include <QString>
#include <QMetaEnum>
#include "entry.h"
#include "../account_global.h"

class ACCOUNT_EXPORT Total : public MetaData
{
    Q_GADGET
    
    Q_PROPERTY(QDate date READ date)
    Q_PROPERTY(double value READ value)
    
private:

public:
    Total();
    Total(const Total&);
	~Total() = default;

	Total& operator = (const Total&);

    friend Total ACCOUNT_EXPORT operator-(const Total &, const Total &);
    friend Total ACCOUNT_EXPORT operator+(const Entry &, const Entry &);
    friend Total ACCOUNT_EXPORT operator+(const Total &, const Total &);
    friend Total ACCOUNT_EXPORT operator+(const Total &, const Entry &);
    friend Total ACCOUNT_EXPORT operator+(const Entry &, const Total &);
    friend bool ACCOUNT_EXPORT operator<(const Total &, const Total &);
    friend bool ACCOUNT_EXPORT operator==(const Total &, const Total &);

    QDate date() const;
    void setDate(QDate date);
    double value() const;
    void setValue(double value);

    static QDate maxDate(const QDate&, const QDate&);
};

Q_DECLARE_METATYPE(Total)
#endif // TOTAL_H
