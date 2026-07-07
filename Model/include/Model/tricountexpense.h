#pragma once

#include "metadata.h"
#include "model_global.h"
#include "transaction.h"
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QObject>
#include <QSharedPointer>
#include <QUuid>

/**
 * @brief A single shared expense inside a Tricount.
 *
 * The monetary data (value, description, date, support, category) is carried
 * by an underlying Transaction, reused rather than duplicated. TricountExpense
 * only adds the split semantics: which member advanced the money (payerId) and
 * which members are excluded from the split (excluded).
 *
 * By default an expense is shared equally between *all* members of the
 * Tricount; members listed in excluded() do not take part. The split is
 * cent-accurate: shareByMember() always sums back to the transaction value,
 * any rounding remainder being spread over the first participants
 * (deterministically ordered by member id).
 */
class MODEL_EXPORT TricountExpense : public QObject, public MetaData {
    Q_OBJECT

    Q_PROPERTY(QUuid id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(TransactionPtr transaction READ transaction WRITE setTransaction NOTIFY transactionChanged)
    Q_PROPERTY(QUuid payerId READ payerId WRITE setPayerId NOTIFY payerIdChanged)
    Q_PROPERTY(QList<QUuid> excluded READ excluded WRITE setExcluded NOTIFY excludedChanged)

public:
    explicit TricountExpense(QObject* parent = nullptr);
    explicit TricountExpense(const QJsonObject& json, QObject* parent = nullptr);
    ~TricountExpense() override = default;

    TricountExpense(const TricountExpense&) = delete;
    TricountExpense& operator=(const TricountExpense&) = delete;
    TricountExpense(TricountExpense&&) = delete;
    TricountExpense& operator=(TricountExpense&&) = delete;

    // Accessors
    [[nodiscard]] QUuid id() const { return metaData<QUuid>(Key::id); }
    [[nodiscard]] TransactionPtr transaction() const { return m_transaction; }
    [[nodiscard]] QUuid payerId() const { return metaData<QUuid>(Key::payerId); }
    [[nodiscard]] QList<QUuid> excluded() const {
        return metaData<QList<QUuid>>(Key::excluded);
    }

    /** Convenience: the absolute monetary amount of the underlying transaction. */
    [[nodiscard]] double amount() const;

    /** True when amount > 0 and a payer is set. */
    [[nodiscard]] bool isValid() const;

    /**
     * @brief Members that actually share this expense: memberIds minus excluded(),
     *        order preserved. Falls back to {payerId} when the result is empty.
     */
    [[nodiscard]] QList<QUuid> participants(const QList<QUuid>& memberIds) const;

    /**
     * @brief Amount owed by each participant, keyed by member id.
     *
     * Equal split among participants(memberIds), cent-accurate: the values sum
     * exactly to amount(). Returns an empty map when the expense is invalid.
     */
    [[nodiscard]] QMap<QUuid, double> shareByMember(const QList<QUuid>& memberIds) const;

    [[nodiscard]] QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

public slots:
    void setId(QUuid v);
    void setTransaction(TransactionPtr v);
    void setPayerId(QUuid v);
    void setExcluded(QList<QUuid> v);

signals:
    void idChanged();
    void transactionChanged();
    void payerIdChanged();
    void excludedChanged();
    void changed();

private:
    struct Key {
        static constexpr auto id = "id";
        static constexpr auto transaction = "transaction";
        static constexpr auto payerId = "payerId";
        static constexpr auto excluded = "excluded";
    };

    static QJsonArray uuidsToJson(const QList<QUuid>& uuids);
    static QList<QUuid> uuidsFromJson(const QJsonArray& arr);

    TransactionPtr m_transaction;
};

using TricountExpensePtr = QSharedPointer<TricountExpense>;
Q_DECLARE_METATYPE(TricountExpensePtr)
