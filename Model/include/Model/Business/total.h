#pragma once

#include <QList>
#include <QDate>
#include <QSharedPointer>
#include "model_global.h"
#include "transaction.h"
#include "enums.h"

// Total is a lightweight value-type summarizing a set of transactions.
// It stores shared pointers to Transactions to avoid copying QObject-derived objects.
class MODEL_EXPORT Total
{
public:
    Total() = default;

    QDate from() const { return m_from; }
    QDate to()   const { return m_to; }

    // Add one or many transactions (shared ownership).
    void addTransaction(QSharedPointer<Transaction> t);
    void addTransactions(const QList<QSharedPointer<Transaction>>& list);

    // Access the stored shared pointers.
    QList<QSharedPointer<Transaction>> transactions() const { return m_transactions; }

    // Evaluate the signed amount using Movement rule:
    // Credit => +value, Debit => -value
    double evaluate() const;

    // Merge operators (non-mutating)
    friend Total MODEL_EXPORT operator+(const Total& a, const Total& b);
    friend Total MODEL_EXPORT operator+(const Total& a, const QSharedPointer<Transaction>& t);
    friend Total MODEL_EXPORT operator+(const Total& a, Transaction& t);

private:
    void recomputeBoundsAfterAppend(const QDate& d);

private:
    QList<QSharedPointer<Transaction>> m_transactions;
    QDate m_from;
    QDate m_to;
};
