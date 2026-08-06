#pragma once

#include "metadata.h"
#include "model_global.h"
#include "transaction.h"
#include "tricountexpense.h"
#include "tricountmember.h"
#include <QDate>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QMetaType>
#include <QObject>
#include <QString>
#include <QUuid>

/**
 * @brief A single money transfer needed to settle a Tricount.
 *
 * `from` owes `amount` to `to`. Exposed as a gadget so QML can read the
 * fields directly.
 */
struct MODEL_EXPORT TricountSettlement {
    Q_GADGET
    Q_PROPERTY(QUuid from MEMBER from)
    Q_PROPERTY(QUuid to MEMBER to)
    Q_PROPERTY(double amount MEMBER amount)

public:
    QUuid from;
    QUuid to;
    double amount = 0.0;

    bool operator==(const TricountSettlement& other) const;
};

/**
 * @brief An expense-sharing event: members and the expenses they share.
 *
 * Members may be OpenBudget profiles or guests (see TricountMember). Each
 * expense is advanced by one member and split between participants. The
 * Tricount computes the net balance of every member, the minimal set of
 * transfers to settle up, and the Transactions to attach to the real accounts
 * of profile-linked members.
 */
class MODEL_EXPORT Tricount : public QObject, public MetaData {
    Q_OBJECT

    Q_PROPERTY(QUuid id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(QString currency READ currency WRITE setCurrency NOTIFY currencyChanged)
    Q_PROPERTY(QList<TricountMemberPtr> members READ members WRITE setMembers NOTIFY membersChanged)
    Q_PROPERTY(QList<TricountExpensePtr> expenses READ expenses WRITE setExpenses NOTIFY expensesChanged)

public:
    explicit Tricount(QObject* parent = nullptr);
    explicit Tricount(const QJsonObject& json, QObject* parent = nullptr);
    ~Tricount() override = default;

    Tricount(const Tricount&) = delete;
    Tricount& operator=(const Tricount&) = delete;
    Tricount(Tricount&&) = delete;
    Tricount& operator=(Tricount&&) = delete;

    // Accessors
    [[nodiscard]] QUuid id() const { return metaData<QUuid>(Key::id); }
    [[nodiscard]] QString name() const { return metaData<QString>(Key::name); }
    [[nodiscard]] QString description() const { return metaData<QString>(Key::description); }
    [[nodiscard]] QDate date() const { return metaData<QDate>(Key::date); }
    [[nodiscard]] QString currency() const { return metaData<QString>(Key::currency); }
    [[nodiscard]] QList<TricountMemberPtr> members() const { return m_members; }
    [[nodiscard]] QList<TricountExpensePtr> expenses() const { return m_expenses; }

    /** Lookup a member by id; null pointer when absent. */
    [[nodiscard]] TricountMemberPtr member(QUuid id) const;

    /**
     * @brief Net balance of every member, keyed by member id.
     *
     * Positive => the member is owed money (advanced more than their share);
     * negative => the member owes money. Balances always sum to zero.
     */
    [[nodiscard]] QMap<QUuid, double> balances() const;

    /**
     * @brief Minimal set of transfers that settles every balance to zero.
     *
     * Greedy largest-creditor / largest-debtor matching. Amounts are positive.
     */
    [[nodiscard]] QList<TricountSettlement> settlements() const;

    /**
     * @brief Transactions to attach to the accounts of profile-linked members.
     *
     * For each expense, the payer's real outflow is emitted as a Debit on the
     * payer account (when the payer is profile-linked and has an accountId).
     * For each settlement, the reimbursement is emitted as a Transfer: a Debit
     * on the debtor account and a Credit on the creditor account, for members
     * that carry an accountId. Guests (no account) produce no transaction.
     *
     * Every generated transaction is tagged with the "tricountId" metadata key.
     */
    [[nodiscard]] QList<TransactionPtr> generateTransactions() const;

    [[nodiscard]] QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

public slots:
    void setId(QUuid v);
    void setName(QString v);
    void setDescription(QString v);
    void setDate(QDate v);
    void setCurrency(QString v);
    void setMembers(QList<TricountMemberPtr> v);
    void setExpenses(QList<TricountExpensePtr> v);

    void addMember(TricountMemberPtr member);
    void removeMember(QUuid id);
    void addExpense(TricountExpensePtr expense);
    void removeExpense(QUuid id);

signals:
    void idChanged();
    void nameChanged();
    void descriptionChanged();
    void dateChanged();
    void currencyChanged();
    void membersChanged();
    void expensesChanged();
    void changed();

private:
    struct Key {
        static constexpr auto id = "id";
        static constexpr auto name = "name";
        static constexpr auto description = "description";
        static constexpr auto date = "date";
        static constexpr auto currency = "currency";
        static constexpr auto members = "members";
        static constexpr auto expenses = "expenses";
        static constexpr auto tricountId = "tricountId";
    };

    QList<TricountMemberPtr> m_members;
    QList<TricountExpensePtr> m_expenses;
};

Q_DECLARE_METATYPE(TricountSettlement)
Q_DECLARE_METATYPE(QList<TricountSettlement>)
Q_DECLARE_METATYPE(QList<TricountMemberPtr>)
Q_DECLARE_METATYPE(QList<TricountExpensePtr>)
