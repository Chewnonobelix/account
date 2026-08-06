#pragma once

#include "metadata.h"
#include "model_global.h"
#include "transaction.h"

#include <QDate>
#include <QList>
#include <QObject>
#include <QString>
#include <QUuid>

/**
 * @brief A single money movement from one account to another.
 *
 * Holds the debited account, the credited account, the amount, the date
 * and a description. generateTransactions() turns it into the two linked
 * Transaction legs (a Debit on the debited account, a Credit on the
 * credited account), both tagged Support::Transfer.
 */
class MODEL_EXPORT Transfer : public QObject, public MetaData {
    Q_OBJECT

    Q_PROPERTY(QUuid debitAccountId READ debitAccountId WRITE setDebitAccountId NOTIFY debitAccountIdChanged)
    Q_PROPERTY(QUuid creditAccountId READ creditAccountId WRITE setCreditAccountId NOTIFY creditAccountIdChanged)
    Q_PROPERTY(double amount READ amount WRITE setAmount NOTIFY amountChanged)
    Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

public:
    explicit Transfer(QObject* parent = nullptr);
    ~Transfer() override = default;

    Transfer(const Transfer&) = delete;
    Transfer& operator=(const Transfer&) = delete;
    Transfer(Transfer&&) = delete;
    Transfer& operator=(Transfer&&) = delete;

    // Accessors
    [[nodiscard]] QUuid debitAccountId() const { return metaData<QUuid>(Key::debitAccountId); }
    [[nodiscard]] QUuid creditAccountId() const { return metaData<QUuid>(Key::creditAccountId); }
    [[nodiscard]] double amount() const { return metaData<double>(Key::amount); }
    [[nodiscard]] QDate date() const { return metaData<QDate>(Key::date); }
    [[nodiscard]] QString description() const { return metaData<QString>(Key::description); }

    /**
     * @brief True when the debit/credit accounts are distinct and set, the
     *        amount is strictly positive and the date is valid.
     */
    [[nodiscard]] bool isValid() const;

    /**
     * @brief Builds the two linked Transaction legs for this transfer.
     *
     * Returns a Debit transaction on debitAccountId() and a Credit
     * transaction on creditAccountId(), both Support::Transfer. Returns an
     * empty list when isValid() is false.
     */
    [[nodiscard]] QList<TransactionPtr> generateTransactions() const;

public slots:
    void setDebitAccountId(QUuid v);
    void setCreditAccountId(QUuid v);
    void setAmount(double v);
    void setDate(QDate v);
    void setDescription(QString v);

signals:
    void debitAccountIdChanged();
    void creditAccountIdChanged();
    void amountChanged();
    void dateChanged();
    void descriptionChanged();
    void changed();

private:
    struct Key {
        static constexpr auto debitAccountId = "debitAccountId";
        static constexpr auto creditAccountId = "creditAccountId";
        static constexpr auto amount = "amount";
        static constexpr auto date = "date";
        static constexpr auto description = "description";
    };
};
