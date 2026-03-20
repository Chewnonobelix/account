#include "Model/enums.h"
#include <QDate>

int OpenAccountEnums::dayToNext(QDate start, Frequency frequency) {
  switch (frequency) {
  case Frequency::Once:
    return 0;

  case Frequency::Daily:
    return start.daysTo(start.addDays(1));

  case Frequency::Weekly:
    return start.daysTo(start.addDays(7));

  case Frequency::Monthly:
    return start.daysTo(start.addMonths(1));

  case Frequency::Quarterly:
    return start.daysTo(start.addMonths(3));

  case Frequency::Yearly:
    return start.daysTo(start.addYears(1));

  case Frequency::Custom:
    return 0; // handled outside
  default:
    break;
  }

  return 0;
}