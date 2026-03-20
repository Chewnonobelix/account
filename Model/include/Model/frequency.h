#pragma once

#include <QDate>
#include <QEnableSharedFromThis>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QUuid>
#include <QVariant>

#include "enums.h"
#include "metadata.h"
#include "transaction.h"

/**
 * @brief Generates dated transactions from a stored prototype transaction.
 */
class Frequency : public QObject,
                  public MetaData,
                  public QEnableSharedFromThis<Frequency>
{
    Q_OBJECT

    Q_PROPERTY(QUuid id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(OpenAccountEnums::Frequency frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
    Q_PROPERTY(QString dateFormat READ dateFormat WRITE setDateFormat NOTIFY dateFormatChanged)
    Q_PROPERTY(int customIntervalDays READ customIntervalDays WRITE setCustomIntervalDays NOTIFY customIntervalDaysChanged)
    Q_PROPERTY(TransactionPtr prototype READ prototype WRITE setPrototype NOTIFY prototypeChanged)

public:
    explicit Frequency(QObject *parent = nullptr);
    explicit Frequency(const QJsonObject &json, QObject *parent = nullptr);

    Frequency(const Frequency &) = delete;
    Frequency &operator=(const Frequency &) = delete;

    QUuid id() const;
    OpenAccountEnums::Frequency frequency() const;
    QString dateFormat() const;
    int customIntervalDays() const;
    TransactionPtr prototype() const;

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

public:
    QJsonObject toJson() const;

    QList<TransactionPtr> generate(QDate from, QDate to) const;
    QList<TransactionPtr> generate(QDate startDate, int occurrenceCount) const;

private:
    TransactionPtr createTransaction(TransactionPtr prototype, QDate date) const;

    QString buildDescription(QString baseDescription, QDate date) const;
    QDate nextDate(QDate current) const;

    template<typename T, typename Signal>
    void assignIfChanged(const char *key, T value, Signal signal)
    {
        if (metaData<T>(key) == value)
            return;

        setMetadata(key, QVariant::fromValue(value));
        emit (this->*signal)();
    }

private:
    struct Key {
        static constexpr auto Id = "id";
        static constexpr auto Frequency = "frequency";
        static constexpr auto DateFormat = "dateFormat";
        static constexpr auto CustomIntervalDays = "customIntervalDays";
        static constexpr auto Prototype = "prototype";
    };

    struct TransactionKey {
        static constexpr auto FrequencyId = "frequencyId";
    };
};

using FrequencyPtr = QSharedPointer<Frequency>;
Q_DECLARE_METATYPE(FrequencyPtr)
