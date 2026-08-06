#include "Model/Models/accounttransactionfilterproxymodel.h"

#include "Model/Models/transactionlistmodel.h"

#include <QString>

AccountTransactionFilterProxyModel::AccountTransactionFilterProxyModel(
    QObject *parent)
    : QSortFilterProxyModel(parent) {
  m_descriptionFilter.setPatternOptions(
      QRegularExpression::CaseInsensitiveOption);

  connect(this, &QAbstractItemModel::rowsInserted, this,
          &AccountTransactionFilterProxyModel::countChanged);
  connect(this, &QAbstractItemModel::rowsRemoved, this,
          &AccountTransactionFilterProxyModel::countChanged);
  connect(this, &QAbstractItemModel::modelReset, this,
          &AccountTransactionFilterProxyModel::countChanged);
}

void AccountTransactionFilterProxyModel::setAccount(Account *account) {
  if (m_account == account)
    return;

  if (m_account)
    disconnect(m_account, nullptr, this, nullptr);

  m_account = account;

  if (m_account) {
    connect(m_account, &Account::changed, this,
            &AccountTransactionFilterProxyModel::invalidateAccountFilter);
    connect(m_account, &QObject::destroyed, this, [this]() {
      m_account = nullptr;
      emit accountChanged();
      invalidateAccountFilter();
    });
  }

  emit accountChanged();
  invalidateAccountFilter();
}

void AccountTransactionFilterProxyModel::setDateFilter(
    const QList<QDate> &dateFilter) {
  if (m_dateFilter == dateFilter)
    return;

  m_dateFilter = dateFilter;
  emit dateFilterChanged();
  invalidateFilter();
}

void AccountTransactionFilterProxyModel::setSupportFilter(
    const QList<OpenAccountEnums::Support> &supportFilter) {
  if (m_supportFilter == supportFilter)
    return;

  m_supportFilter = supportFilter;
  emit supportFilterChanged();
  invalidateFilter();
}

void AccountTransactionFilterProxyModel::setCategoryFilter(
    const QList<QUuid> &categoryFilter) {
  if (m_categoryFilter == categoryFilter)
    return;

  m_categoryFilter = categoryFilter;
  emit categoryFilterChanged();
  invalidateFilter();
}

void AccountTransactionFilterProxyModel::setDescriptionFilter(
    const QString &descriptionFilter) {
  if (m_descriptionFilterText == descriptionFilter)
    return;

  m_descriptionFilterText = descriptionFilter;
  m_descriptionFilter.setPattern(
      QRegularExpression::escape(descriptionFilter));
  emit descriptionFilterChanged();
  invalidateFilter();
}

QHash<int, QByteArray> AccountTransactionFilterProxyModel::roleNames() const {
  return sourceModel() ? sourceModel()->roleNames()
                       : QSortFilterProxyModel::roleNames();
}

QObject *AccountTransactionFilterProxyModel::at(int row) const {
  const QModelIndex proxyIndex = index(row, 0);
  if (!proxyIndex.isValid())
    return nullptr;

  return qvariant_cast<QObject *>(
      data(proxyIndex, TransactionListModel::TransactionRole));
}

QVariantMap AccountTransactionFilterProxyModel::get(int row) const {
  const QModelIndex proxyIndex = index(row, 0);
  if (!proxyIndex.isValid())
    return {};

  QVariantMap result;
  const QHash<int, QByteArray> names = roleNames();
  for (auto it = names.cbegin(); it != names.cend(); ++it)
    result.insert(QString::fromLatin1(it.value()), data(proxyIndex, it.key()));

  return result;
}

bool AccountTransactionFilterProxyModel::filterAcceptsRow(
    int sourceRow, const QModelIndex &sourceParent) const {
  if (!sourceModel())
    return false;

  const QModelIndex sourceIndex = sourceModel()->index(sourceRow, 0, sourceParent);
  if (!sourceIndex.isValid())
    return false;

  if (m_account &&
      sourceModel()
              ->data(sourceIndex, TransactionListModel::AccountIdRole)
              .toUuid() != m_account->id())
    return false;

  if (!m_dateFilter.isEmpty() &&
      !m_dateFilter.contains(
          sourceModel()->data(sourceIndex, TransactionListModel::DateRole).toDate()))
    return false;

  if (!m_supportFilter.isEmpty() &&
      !m_supportFilter.contains(
          sourceModel()
              ->data(sourceIndex, TransactionListModel::SupportRole)
              .value<OpenAccountEnums::Support>()))
    return false;

  if (!m_categoryFilter.isEmpty() &&
      !m_categoryFilter.contains(
          sourceModel()
              ->data(sourceIndex, TransactionListModel::CategoryRole)
              .toUuid()))
    return false;

  if (!m_descriptionFilterText.isEmpty()) {
    const QString description =
        sourceModel()
            ->data(sourceIndex, TransactionListModel::DescriptionRole)
            .toString();
    if (!m_descriptionFilter.isValid() ||
        !m_descriptionFilter.match(description).hasMatch())
      return false;
  }

  return true;
}

void AccountTransactionFilterProxyModel::invalidateAccountFilter() {
  invalidateFilter();
}

void AccountTransactionFilterProxyModel::invalidateFilter() {
  beginFilterChange();
  endFilterChange(QSortFilterProxyModel::Direction::Rows);
}
