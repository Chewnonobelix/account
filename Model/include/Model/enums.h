#pragma once

#include <QObject>

namespace OpenAccountEnums {

/**
 * @brief Represents the type of movement (OP-34)
 */
enum class Movement {
    Credit,
    Debit
};

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

} // namespace OpenAccountEnums

Q_DECLARE_METATYPE(OpenAccountEnums::Movement)
Q_DECLARE_METATYPE(OpenAccountEnums::Support)
Q_DECLARE_METATYPE(OpenAccountEnums::Frequency)
