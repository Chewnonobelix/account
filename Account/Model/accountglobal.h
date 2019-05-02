#ifndef ACCOUNTGLOBAL_H
#define ACCOUNTGLOBAL_H

#include <QObject>

namespace Account
{
    Q_NAMESPACE

    enum class FrequencyEnum {unique, day, week, month, quarter, year};
    Q_ENUM_NS(FrequencyEnum)

    enum TypeEnum {outcome = -1, income = 1};
    Q_ENUM_NS(TypeEnum)
}

#endif // ACCOUNTGLOBAL_H
