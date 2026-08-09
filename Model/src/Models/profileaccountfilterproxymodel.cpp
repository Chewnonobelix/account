#include "Model/Models/profileaccountfilterproxymodel.h"

#include "Model/Models/accountlistmodel.h"

#include <QString>

ProfileAccountFilterProxyModel::ProfileAccountFilterProxyModel(
    QObject *parent)
    : QSortFilterProxyModel(parent) {
  connect(this, &QAbstractItemModel::rowsInserted, this,
          &ProfileAccountFilterProxyModel::countChanged);
  connect(this, &QAbstractItemModel::rowsRemoved, this,
          &ProfileAccountFilterProxyModel::countChanged);
  connect(this, &QAbstractItemModel::modelReset, this,
          &ProfileAccountFilterProxyModel::countChanged);
}

void ProfileAccountFilterProxyModel::setProfile(Profile *profile) {
  if (m_profile == profile)
    return;

  if (m_profile)
    disconnect(m_profile, nullptr, this, nullptr);

  m_profile = profile;

  if (m_profile) {
    connect(m_profile, &Profile::accountsChanged, this,
            &ProfileAccountFilterProxyModel::invalidateProfileFilter);
    connect(m_profile, &QObject::destroyed, this, [this]() {
      m_profile = nullptr;
      emit profileChanged();
      invalidateProfileFilter();
    });
  }

  emit profileChanged();
  invalidateProfileFilter();
}

QHash<int, QByteArray> ProfileAccountFilterProxyModel::roleNames() const {
  return sourceModel() ? sourceModel()->roleNames()
                       : QSortFilterProxyModel::roleNames();
}

QObject *ProfileAccountFilterProxyModel::at(int row) const {
  const QModelIndex proxyIndex = index(row, 0);
  if (!proxyIndex.isValid())
    return nullptr;

  return qvariant_cast<QObject *>(
      data(proxyIndex, AccountListModel::AccountRole));
}

QVariantMap ProfileAccountFilterProxyModel::get(int row) const {
  const QModelIndex proxyIndex = index(row, 0);
  if (!proxyIndex.isValid())
    return {};

  QVariantMap result;
  const QHash<int, QByteArray> names = roleNames();
  for (auto it = names.cbegin(); it != names.cend(); ++it)
    result.insert(QString::fromLatin1(it.value()), data(proxyIndex, it.key()));

  return result;
}

bool ProfileAccountFilterProxyModel::filterAcceptsRow(
    int sourceRow, const QModelIndex &sourceParent) const {
  if (!sourceModel() || !m_profile)
    return false;

  const QModelIndex sourceIndex = sourceModel()->index(sourceRow, 0, sourceParent);
  if (!sourceIndex.isValid())
    return false;

  return m_profile->accounts().contains(
      sourceModel()->data(sourceIndex, AccountListModel::IdRole).toUuid());
}

void ProfileAccountFilterProxyModel::invalidateProfileFilter() {
  beginFilterChange();
  endFilterChange(QSortFilterProxyModel::Direction::Rows);
}
