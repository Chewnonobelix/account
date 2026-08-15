#include "Model/Models/estimatedtransactionfilterproxymodel.h"

#include "Model/Models/transactionlistmodel.h"

EstimatedTransactionFilterProxyModel::EstimatedTransactionFilterProxyModel(
    QObject *parent)
    : QSortFilterProxyModel(parent), m_referenceDate(QDate::currentDate()) {
  connect(this, &QAbstractItemModel::rowsInserted, this,
          &EstimatedTransactionFilterProxyModel::countChanged);
  connect(this, &QAbstractItemModel::rowsRemoved, this,
          &EstimatedTransactionFilterProxyModel::countChanged);
  connect(this, &QAbstractItemModel::modelReset, this,
          &EstimatedTransactionFilterProxyModel::countChanged);
}

void EstimatedTransactionFilterProxyModel::setReferenceDate(QDate date) {
  if (m_referenceDate == date)
    return;

  m_referenceDate = date;
  emit referenceDateChanged();
  invalidateFilter();
}

QHash<int, QByteArray> EstimatedTransactionFilterProxyModel::roleNames() const {
  return sourceModel() ? sourceModel()->roleNames()
                       : QSortFilterProxyModel::roleNames();
}

QObject *EstimatedTransactionFilterProxyModel::at(int row) const {
  const QModelIndex proxyIndex = index(row, 0);
  if (!proxyIndex.isValid())
    return nullptr;

  return qvariant_cast<QObject *>(
      data(proxyIndex, TransactionListModel::TransactionRole));
}

QVariantMap EstimatedTransactionFilterProxyModel::get(int row) const {
  const QModelIndex proxyIndex = index(row, 0);
  if (!proxyIndex.isValid())
    return {};

  QVariantMap result;
  const QHash<int, QByteArray> names = roleNames();
  for (auto it = names.cbegin(); it != names.cend(); ++it)
    result.insert(QString::fromLatin1(it.value()), data(proxyIndex, it.key()));

  return result;
}

bool EstimatedTransactionFilterProxyModel::filterAcceptsRow(
    int sourceRow, const QModelIndex &sourceParent) const {
  if (!sourceModel())
    return false;

  const QModelIndex sourceIndex =
      sourceModel()->index(sourceRow, 0, sourceParent);
  if (!sourceIndex.isValid())
    return false;

  if (!sourceModel()
           ->data(sourceIndex, TransactionListModel::EstimatedRole)
           .toBool())
    return false;

  const QDate date =
      sourceModel()->data(sourceIndex, TransactionListModel::DateRole).toDate();
  return date.isValid() && date < m_referenceDate;
}

void EstimatedTransactionFilterProxyModel::invalidateFilter() {
  beginFilterChange();
  endFilterChange(QSortFilterProxyModel::Direction::Rows);
}
