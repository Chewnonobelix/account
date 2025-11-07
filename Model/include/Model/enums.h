#pragma once

#include <QObject>

namespace OpenAccountEnums {
Q_NAMESPACE
/**
 * @brief Represents the type of movement (OP-34)
 */
enum class Movement {
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
