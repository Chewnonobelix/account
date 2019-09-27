#include "accountglobal.h"

int Account::nbDay(QDate d, FrequencyEnum f)
{
    int ret;
    QDate t;
    
    switch(f)
    {
    case FrequencyEnum::day:
        ret = 1;
        break;
        
    case FrequencyEnum::week:
        ret = 7;
        break;
        
    case FrequencyEnum::month:
        t = d.addMonths(1);
        ret = d.daysTo(t);
        break;
        
    case FrequencyEnum::quarter:
        t = d.addMonths(3);
        ret = d.daysTo(t);
        break;
        
    case FrequencyEnum::year:
        t = d.addYears(1);
        ret = d.daysTo(t);
        break;
        
    default:
        ret = 0;
    }
    
    return ret;
}
