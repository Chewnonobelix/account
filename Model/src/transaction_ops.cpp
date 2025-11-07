#include "Model/transaction.h"
#include "Model/total.h"
#include <QtGlobal>

static inline QSharedPointer<Transaction> ensureShared(Transaction& t, const char* where)
{
    auto sp = t.sharedFromThis();
    Q_ASSERT_X(!sp.isNull(), where, "Transaction must be owned by a QSharedPointer<Transaction>");
    return sp;
}

Total operator+(Transaction& a, Transaction& b)
{
    Total out;
    out.addTransaction(ensureShared(a, "Transaction::operator+(Transaction,Transaction)"));
    out.addTransaction(ensureShared(b, "Transaction::operator+(Transaction,Transaction)"));
    return out;
}

Total operator+(Transaction& a, const Total& b)
{
    Total out = b;
    out.addTransaction(ensureShared(a, "Transaction::operator+(Transaction,Total)"));
    return out;
}
