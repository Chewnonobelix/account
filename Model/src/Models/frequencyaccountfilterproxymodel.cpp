#include "Model/Models/frequencyaccountfilterproxymodel.h"

#include "Model/Models/frequencylistmodel.h"

#include <QString>

FrequencyAccountFilterProxyModel::FrequencyAccountFilterProxyModel(
    QObject *parent)
    : QSortFilterProxyModel(parent) {
  connect(this, &QAbstractItemModel::rowsInserted, this,
          &FrequencyAccountFilterProxyModel::countChanged);
  connect(this, &QAbstractItemModel::rowsRemoved, this,
          &FrequencyAccountFilterProxyModel::countChanged);
  connect(this, &QAbstractItemModel::modelReset, this,
          &FrequencyAccountFilterProxyModel::countChanged);
}

void FrequencyAccountFilterProxyModel::setAccount(Account *account) {
  if (m_account == account)
    return;

  if (m_account)
    disconnect(m_account, nullptr, this, nullptr);

  m_account = account;

  if (m_account) {
    connect(m_account, &Account::changed, this,
            &FrequencyAccountFilterProxyModel::invalidateAccountFilter);
    connect(m_account, &QObject::destroyed, this, [this]() {
      m_account = nullptr;
      emit accountChanged();
      invalidateAccountFilter();
    });
  }

  emit accountChanged();
  invalidateAccountFilter();
}

QHash<int, QByteArray> FrequencyAccountFilterProxyModel::roleNames() const {
  return sourceModel() ? sourceModel()->roleNames()
                       : QSortFilterProxyModel::roleNames();
}

QObject *FrequencyAccountFilterProxyModel::at(int row) const {
  const QModelIndex proxyIndex = index(row, 0);
  if (!proxyIndex.isValid())
    return nullptr;

  return qvariant_cast<QObject *>(
      data(proxyIndex, FrequencyListModel::FrequencyRole));
}

QVariantMap FrequencyAccountFilterProxyModel::get(int row) const {
  const QModelIndex proxyIndex = index(row, 0);
  if (!proxyIndex.isValid())
    return {};

  QVariantMap result;
  const QHash<int, QByteArray> names = roleNames();
  for (auto it = names.cbegin(); it != names.cend(); ++it)
    result.insert(QString::fromLatin1(it.value()), data(proxyIndex, it.key()));

  return result;
}

bool FrequencyAccountFilterProxyModel::filterAcceptsRow(
    int sourceRow, const QModelIndex &sourceParent) const {
  if (!sourceModel())
    return false;

  if (!m_account)
    return true;

  const QModelIndex sourceIndex = sourceModel()->index(sourceRow, 0, sourceParent);
  if (!sourceIndex.isValid())
    return false;

  const TransactionPtr prototype =
      sourceModel()
          ->data(sourceIndex, FrequencyListModel::PrototypeRole)
          .value<TransactionPtr>();

  return prototype && prototype->accountId() == m_account->id();
}

void FrequencyAccountFilterProxyModel::invalidateAccountFilter() {
  beginFilterChange();
  endFilterChange(QSortFilterProxyModel::Direction::Rows);
}
