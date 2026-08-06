#pragma once

#include "enums.h"
#include "metadata.h"
#include "model_global.h"
#include "transaction.h"

#include <QDate>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QString>
#include <QUuid>

/**
 * @brief Tracks spending against a Category over a period, up to a threshold.
 *
 * A Budget is defined between startDate() and endDate() and targets a single
 * categoryId(). recurrence() == Once makes it a one-off budget; any other
 * value marks it repeatable. Transactions matching the category and period
 * are attached via addTransaction() to compute usedAmount() / usageRatio().
 */
class MODEL_EXPORT Budget : public QObject, public MetaData {
    Q_OBJECT

    Q_PROPERTY(QUuid id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QUuid categoryId READ categoryId WRITE setCategoryId NOTIFY categoryIdChanged)
    Q_PROPERTY(double threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged)
    Q_PROPERTY(QDate startDate READ startDate WRITE setStartDate NOTIFY startDateChanged)
    Q_PROPERTY(QDate endDate READ endDate WRITE setEndDate NOTIFY endDateChanged)
    Q_PROPERTY(OpenAccountEnums::Frequency recurrence READ recurrence WRITE setRecurrence NOTIFY recurrenceChanged)
    Q_PROPERTY(bool repeatable READ isRepeatable NOTIFY recurrenceChanged)
    Q_PROPERTY(double usedAmount READ usedAmount NOTIFY usageChanged)
    Q_PROPERTY(double remaining READ remaining NOTIFY usageChanged)
    Q_PROPERTY(double usageRatio READ usageRatio NOTIFY usageChanged) // percentage, 0.0-100.0
    Q_PROPERTY(bool overBudget READ isOverBudget NOTIFY usageChanged)

public:
    explicit Budget(QObject* parent = nullptr);
    explicit Budget(const QJsonObject& json, QObject* parent = nullptr);
    ~Budget() override = default;

    Budget(const Budget&) = delete;
    Budget& operator=(const Budget&) = delete;
    Budget(Budget&&) = delete;
    Budget& operator=(Budget&&) = delete;

    // Accessors
    [[nodiscard]] QUuid id() const { return metaData<QUuid>(Key::id); }
    [[nodiscard]] QString name() const { return metaData<QString>(Key::name); }
    [[nodiscard]] QUuid categoryId() const { return metaData<QUuid>(Key::categoryId); }
    [[nodiscard]] double threshold() const { return metaData<double>(Key::threshold); }
    [[nodiscard]] QDate startDate() const { return metaData<QDate>(Key::startDate); }
    [[nodiscard]] QDate endDate() const { return metaData<QDate>(Key::endDate); }
    [[nodiscard]] OpenAccountEnums::Frequency recurrence() const {
        return metaData<OpenAccountEnums::Frequency>(Key::recurrence);
    }
    [[nodiscard]] bool isRepeatable() const {
        return recurrence() != OpenAccountEnums::Frequency::Once;
    }

    /** True when date falls within [startDate, endDate] (both bounds must be valid). */
    [[nodiscard]] bool isWithinPeriod(QDate date) const;

    /** True when the transaction's category and date fall within this budget's scope. */
    [[nodiscard]] bool matches(const TransactionPtr& transaction) const;

    /** Attaches transaction if matches() and not already present; returns whether it was added. */
    bool addTransaction(const TransactionPtr& transaction);
    void removeTransaction(QUuid transactionId);
    void clearTransactions();
    [[nodiscard]] QList<TransactionPtr> transactions() const { return m_transactions; }

    /** Sum of the absolute value of every attached transaction. */
    [[nodiscard]] double usedAmount() const;
    /** threshold() - usedAmount(); goes negative once over budget. */
    [[nodiscard]] double remaining() const;
    /** usedAmount() / threshold() * 100, as a percentage; 0 when threshold() <= 0. */
    [[nodiscard]] double usageRatio() const;
    [[nodiscard]] bool isOverBudget() const;

    [[nodiscard]] QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

public slots:
    void setId(QUuid value);
    void setName(QString value);
    void setCategoryId(QUuid value);
    void setThreshold(double value);
    void setStartDate(QDate value);
    void setEndDate(QDate value);
    void setRecurrence(OpenAccountEnums::Frequency value);

signals:
    void idChanged();
    void nameChanged();
    void categoryIdChanged();
    void thresholdChanged();
    void startDateChanged();
    void endDateChanged();
    void recurrenceChanged();
    void usageChanged();
    void changed();

private:
    struct Key {
        static constexpr auto id = "id";
        static constexpr auto name = "name";
        static constexpr auto categoryId = "categoryId";
        static constexpr auto threshold = "threshold";
        static constexpr auto startDate = "startDate";
        static constexpr auto endDate = "endDate";
        static constexpr auto recurrence = "recurrence";
    };

    // Runtime aggregate only: not persisted through MetaData/JSON, mirrors
    // how Total accumulates transactions outside of the metadata map.
    QList<TransactionPtr> m_transactions;
};
