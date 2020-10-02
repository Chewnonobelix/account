#ifndef ACCOUNTGLOBAL_H
#define ACCOUNTGLOBAL_H

#include <QObject>
#include <QDate>
#include "account_global.h"

namespace Account
{
    Q_NAMESPACE
    Q_NAMESPACE_EXPORT(ACCOUNT_EXPORT)

    enum class FrequencyEnum {Unique = 0, Day = 1, Week = 2, Month = 3, Quarter = 4, Year = 5};
    Q_ENUM_NS(FrequencyEnum)

    enum TypeEnum {outcome = -1, income = 1};
    Q_ENUM_NS(TypeEnum)
    
    enum Granularity {Month = 3, Year = 5, Over};
    Q_ENUM_NS(Granularity)

    enum SupportEnum { Money, CB, Transfert, Cheque };
    Q_ENUM_NS(SupportEnum)

    int ACCOUNT_EXPORT nbDay(QDate, FrequencyEnum);
}

#endif // ACCOUNTGLOBAL_H
