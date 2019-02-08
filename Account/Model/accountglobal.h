#ifndef ACCOUNTGLOBAL_H
#define ACCOUNTGLOBAL_H

#include <QObject>
class AccountEnum
{
    Q_GADGET

public:
    enum FrequencyEnum { never, daily, weekly, bimonthly, monthly, annualy};
    Q_ENUM(FrequencyEnum)
};

#endif // ACCOUNTGLOBAL_H
