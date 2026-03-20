#pragma once

#include "model_global.h"
#include <QObject>

namespace OpenAccountEnums {
Q_NAMESPACE_EXPORT(MODEL_EXPORT)
/**
 * @brief Represents the type of movement (OP-34)
 */
enum class Movement { Both, Credit, Debit };
Q_ENUM_NS(Movement)
/**
 * @brief Represents the type of support (OP-35)
 */
enum class Support { Cash, Cheque, Card, Transfer, Other };
Q_ENUM_NS(Support)

/**
 * @brief Represents frequency (OP-36)
 */
enum class Frequency {
  Custom = -1,
  Once = 0,
  Daily = 1,
  Weekly = 7,
  Monthly = 30,
  Quarterly = 120,
  Yearly = 365,
  LAST
};
Q_ENUM_NS(Frequency)

int dayToNext(QDate start, Frequency frequency);
} // namespace OpenAccountEnums

Q_DECLARE_METATYPE(OpenAccountEnums::Movement)
Q_DECLARE_METATYPE(OpenAccountEnums::Support)
Q_DECLARE_METATYPE(OpenAccountEnums::Frequency)
