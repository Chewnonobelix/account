#pragma once

#include "enums.h"
#include "metadata.h"
#include "model_global.h"
#include "transaction.h"
#include <QDate>
#include <QEnableSharedFromThis>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QQmlEngine>
#include <QSharedPointer>
#include <QString>
#include <QUuid>

/**
 * @brief Generates dated transactions from a stored prototype.
 */
class MODEL_EXPORT Frequency : public QObject,
                               public MetaData,
                               public QEnableSharedFromThis<Frequency> {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QUuid id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(OpenAccountEnums::Frequency frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
    Q_PROPERTY(QString dateFormat READ dateFormat WRITE setDateFormat NOTIFY dateFormatChanged)
    Q_PROPERTY(int customIntervalDays READ customIntervalDays WRITE setCustomIntervalDays NOTIFY customIntervalDaysChanged)
    Q_PROPERTY(TransactionPtr prototype READ prototype WRITE setPrototype NOTIFY prototypeChanged)

public:
    explicit Frequency(QObject* parent = nullptr);
    explicit Frequency(const QJsonObject& json, QObject* parent = nullptr);
    ~Frequency() override = default;

    Frequency(const Frequency&) = delete;
    Frequency& operator=(const Frequency&) = delete;
    Frequency(Frequency&&) = delete;
    Frequency& operator=(Frequency&&) = delete;

    [[nodiscard]] QUuid id() const { return metaData<QUuid>(Key::id); }
    [[nodiscard]] OpenAccountEnums::Frequency frequency() const {
        return metaData<OpenAccountEnums::Frequency>(Key::frequency);
    }
    [[nodiscard]] QString dateFormat() const { return metaData<QString>(Key::dateFormat); }
    [[nodiscard]] int customIntervalDays() const { return metaData<int>(Key::customIntervalDays); }
    [[nodiscard]] TransactionPtr prototype() const { return metaData<TransactionPtr>(Key::prototype); }

    [[nodiscard]] QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

    [[nodiscard]] QList<TransactionPtr> generate(QDate from, QDate to) const;
    [[nodiscard]] QList<TransactionPtr> generate(QDate startDate, int occurrenceCount) const;

public slots:
    void setId(QUuid value);
    void setFrequency(OpenAccountEnums::Frequency value);
    void setDateFormat(QString value);
    void setCustomIntervalDays(int value);
    void setPrototype(TransactionPtr value);

signals:
    void idChanged();
    void frequencyChanged();
    void dateFormatChanged();
    void customIntervalDaysChanged();
    void prototypeChanged();
    void changed();

private:
    [[nodiscard]] TransactionPtr createTransaction(TransactionPtr proto, QDate date) const;
    [[nodiscard]] QString buildDescription(const QString& base, QDate date) const;
    [[nodiscard]] QDate nextDate(QDate current) const;

    struct Key {
        static constexpr auto id = "id";
        static constexpr auto frequency = "frequency";
        static constexpr auto dateFormat = "dateFormat";
        static constexpr auto customIntervalDays = "customIntervalDays";
        static constexpr auto prototype = "prototype";
    };

    struct TransactionKey {
        static constexpr auto frequencyId = "frequencyId";
    };
};

using FrequencyPtr = QSharedPointer<Frequency>;
Q_DECLARE_METATYPE(FrequencyPtr)
