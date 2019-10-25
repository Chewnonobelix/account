#include "accountglobal.h"

int Account::nbDay(QDate d, FrequencyEnum f)
{
    int ret;
    QDate t;
    
    switch(f)
    {
    case FrequencyEnum::Day:
        ret = 1;
        break;
        
    case FrequencyEnum::Week:
        ret = 7;
        break;
        
    case FrequencyEnum::Month:
        t = d.addMonths(1);
        ret = d.daysTo(t);
        break;
        
    case FrequencyEnum::Quarter:
        t = d.addMonths(3);
        ret = d.daysTo(t);
        break;
        
    case FrequencyEnum::Year:
        t = d.addYears(1);
        ret = d.daysTo(t);
        break;
        
    default:
        ret = 0;
    }
    
    return ret;
}
