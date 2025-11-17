#pragma once

#include <QObject>
#include "model_global.h"

namespace OpenAccountEnums {
Q_NAMESPACE_EXPORT(MODEL_EXPORT)
/**
 * @brief Represents the type of movement (OP-34)
 */
enum class Movement {
    Both,
    Credit,
    Debit
};
Q_ENUM_NS(Movement)
/**
 * @brief Represents the type of support (OP-35)
 */
enum class Support {
    Cash,
    Cheque,
    Card,
    Transfer,
    Other
};
Q_ENUM_NS(Support)

/**
 * @brief Represents frequency (OP-36)
 */
enum class Frequency {
    Once,
    Daily,
    Weekly,
    Monthly,
    Quarterly,
    Yearly,
    Custom
};
Q_ENUM_NS(Frequency)

} // namespace OpenAccountEnums

Q_DECLARE_METATYPE(OpenAccountEnums::Movement)
Q_DECLARE_METATYPE(OpenAccountEnums::Support)
Q_DECLARE_METATYPE(OpenAccountEnums::Frequency)
