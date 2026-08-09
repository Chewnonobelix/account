#include "Model/Models/debtaccountfilterproxymodel.h"

#include "Model/Models/debtlistmodel.h"

#include <QString>

DebtAccountFilterProxyModel::DebtAccountFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent) {
  connect(this, &QAbstractItemModel::rowsInserted, this,
          &DebtAccountFilterProxyModel::countChanged);
  connect(this, &QAbstractItemModel::rowsRemoved, this,
          &DebtAccountFilterProxyModel::countChanged);
  connect(this, &QAbstractItemModel::modelReset, this,
          &DebtAccountFilterProxyModel::countChanged);
}

void DebtAccountFilterProxyModel::setAccount(Account *account) {
  if (m_account == account)
    return;

  if (m_account)
    disconnect(m_account, nullptr, this, nullptr);

  m_account = account;

  if (m_account) {
    connect(m_account, &Account::changed, this,
            &DebtAccountFilterProxyModel::invalidateAccountFilter);
    connect(m_account, &QObject::destroyed, this, [this]() {
      m_account = nullptr;
      emit accountChanged();
      invalidateAccountFilter();
    });
  }

  emit accountChanged();
  invalidateAccountFilter();
}

QHash<int, QByteArray> DebtAccountFilterProxyModel::roleNames() const {
  return sourceModel() ? sourceModel()->roleNames()
                       : QSortFilterProxyModel::roleNames();
}

QObject *DebtAccountFilterProxyModel::at(int row) const {
  const QModelIndex proxyIndex = index(row, 0);
  if (!proxyIndex.isValid())
    return nullptr;

  return qvariant_cast<QObject *>(data(proxyIndex, DebtListModel::DebtRole));
}

QVariantMap DebtAccountFilterProxyModel::get(int row) const {
  const QModelIndex proxyIndex = index(row, 0);
  if (!proxyIndex.isValid())
    return {};

  QVariantMap result;
  const QHash<int, QByteArray> names = roleNames();
  for (auto it = names.cbegin(); it != names.cend(); ++it)
    result.insert(QString::fromLatin1(it.value()), data(proxyIndex, it.key()));

  return result;
}

bool DebtAccountFilterProxyModel::filterAcceptsRow(
    int sourceRow, const QModelIndex &sourceParent) const {
  if (!sourceModel())
    return false;

  if (!m_account)
    return true;

  const QModelIndex sourceIndex = sourceModel()->index(sourceRow, 0, sourceParent);
  if (!sourceIndex.isValid())
    return false;

  return sourceModel()->data(sourceIndex, DebtListModel::AccountIdRole).toUuid() ==
         m_account->id();
}

void DebtAccountFilterProxyModel::invalidateAccountFilter() {
  beginFilterChange();
  endFilterChange(QSortFilterProxyModel::Direction::Rows);
}
