#include "Model/total.h"
#include <algorithm>

void Total::recomputeBoundsAfterAppend(const QDate &d) {
  if (!m_from.isValid() || d < m_from)
    m_from = d;
  if (!m_to.isValid() || d > m_to)
    m_to = d;
}

void Total::addTransaction(QSharedPointer<Transaction> t) {
  if (!t)
    return;
  m_transactions.push_back(t);
  recomputeBoundsAfterAppend(t->date());
}

void Total::addTransactions(const QVector<QSharedPointer<Transaction>> &list) {
  for (const auto &t : list)
    addTransaction(t);
}

double Total::evaluate() const {
  double sum = 0.0;
  for (const auto &tp : m_transactions) {
    if (!tp)
      continue;
    const double v = tp->value();
    const auto m = tp->movement();
    const double signedVal = (m == OpenAccountEnums::Movement::Credit)  ? (+v)
                             : (m == OpenAccountEnums::Movement::Debit) ? (-v)
                                                                        : v;
    sum += signedVal;
  }
  return sum;
}

Total operator+(const Total &a, const Total &b) {
  Total out;
  out.m_transactions.reserve(a.m_transactions.size() + b.m_transactions.size());
  for (const auto &t : a.m_transactions)
    out.addTransaction(t);
  for (const auto &t : b.m_transactions)
    out.addTransaction(t);
  return out;
}

Total operator+(const Total &a, const QSharedPointer<Transaction> &t) {
  Total out = a;
  out.addTransaction(t);
  return out;
}

Total operator+(const Total &a, Transaction &t) {
  Total out = a;
  auto sp = t.sharedFromThis();
  Q_ASSERT_X(!sp.isNull(), "Total::operator+(Total, Transaction)",
             "Transaction must be owned "
             "by a QSharedPointer");
  out.addTransaction(sp);
  return out;
}
