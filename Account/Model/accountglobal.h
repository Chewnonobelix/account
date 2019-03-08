#ifndef ACCOUNTGLOBAL_H
#define ACCOUNTGLOBAL_H

#include <QObject>

class Account
{
    Q_GADGET
public:
    enum class FrequencyEnum {unique, day, week, month, quarter, year};
    Q_ENUM(FrequencyEnum)
};

#endif // ACCOUNTGLOBAL_H
