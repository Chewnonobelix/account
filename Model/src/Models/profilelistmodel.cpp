#include "Model/Models/profilelistmodel.h"

#include <QSet>
#include <QString>

namespace {

bool uuidFromVariant(const QVariant &value, QUuid *out) {
  if (!out || !value.isValid())
    return false;

  if (value.canConvert<QUuid>()) {
    *out = value.toUuid();
    return true;
  }

  const QString text = value.toString().trimmed();
  if (text.isEmpty()) {
    *out = QUuid();
    return true;
  }

  const QUuid uuid = QUuid::fromString(text);
  if (uuid.isNull())
    return false;

  *out = uuid;
  return true;
}

} // namespace

ProfileListModel::ProfileListModel(QObject *parent)
    : QAbstractListModel(parent) {}

int ProfileListModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;

  return m_profiles.size();
}

QVariant ProfileListModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_profiles.size())
    return {};

  return roleData(m_profiles.at(index.row()), role);
}

bool ProfileListModel::setData(const QModelIndex &index, const QVariant &value,
                               int role) {
  const ProfilePtr profile = profileAt(index.row());
  if (!profile)
    return false;

  switch (role) {
  case IdRole: {
    QUuid id;
    if (!uuidFromVariant(value, &id))
      return false;
    profile->setId(id);
    return true;
  }
  case FirstNameRole:
    profile->setFirstName(value.toString());
    return true;
  case LastNameRole:
  case Qt::EditRole:
    profile->setLastName(value.toString());
    return true;
  default:
    break;
  }

  return false;
}

Qt::ItemFlags ProfileListModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QHash<int, QByteArray> ProfileListModel::roleNames() const {
  return {{ProfileRole, "profile"},
          {IdRole, "id"},
          {FirstNameRole, "firstName"},
          {LastNameRole, "lastName"},
          {AccountsRole, "accounts"}};
}

ProfilePtr ProfileListModel::profileAt(int row) const {
  if (row < 0 || row >= m_profiles.size())
    return {};

  return m_profiles.at(row);
}

bool ProfileListModel::contains(const QUuid &profileId) const {
  return indexOf(profileId) >= 0;
}

void ProfileListModel::setProfiles(const QList<ProfilePtr> &profiles) {
  QList<ProfilePtr> nextProfiles;
  nextProfiles.reserve(profiles.size());

  // Hash-set dedup instead of an O(n) scan per candidate: with a large
  // bulk-loaded list the scan-per-candidate version turns into an O(n^2)
  // pass that can visibly freeze the UI thread this runs on.
  QSet<const Profile *> seenPointers;
  QSet<QUuid> seenIds;
  seenPointers.reserve(profiles.size());
  seenIds.reserve(profiles.size());

  for (const ProfilePtr &profile : profiles) {
    if (!profile || seenPointers.contains(profile.data()))
      continue;
    if (!profile->id().isNull() && seenIds.contains(profile->id()))
      continue;

    seenPointers.insert(profile.data());
    if (!profile->id().isNull())
      seenIds.insert(profile->id());

    nextProfiles.append(profile);
  }

  const int oldCount = m_profiles.size();

  for (const ProfilePtr &profile : m_profiles)
    disconnectProfile(profile.data());

  beginResetModel();
  m_profiles = nextProfiles;
  endResetModel();

  for (const ProfilePtr &profile : m_profiles)
    connectProfile(profile);

  if (oldCount != m_profiles.size())
    emit countChanged();
}

bool ProfileListModel::addProfile(const ProfilePtr &profile) {
  if (!profile || indexOf(profile.data()) >= 0 ||
      (!profile->id().isNull() && contains(profile->id()))) {
    return false;
  }

  const int row = m_profiles.size();
  beginInsertRows(QModelIndex(), row, row);
  m_profiles.append(profile);
  connectProfile(profile);
  endInsertRows();

  emit countChanged();
  return true;
}

bool ProfileListModel::addProfile(QObject *profile) {
  Profile *rawProfile = qobject_cast<Profile *>(profile);
  if (!rawProfile)
    return false;

  return addProfile(ProfilePtr(rawProfile, [](Profile *) {}));
}

bool ProfileListModel::removeProfile(const QUuid &profileId) {
  return removeProfileAt(indexOf(profileId));
}

bool ProfileListModel::removeProfileAt(int row) {
  if (row < 0 || row >= m_profiles.size())
    return false;

  disconnectProfile(m_profiles.at(row).data());

  beginRemoveRows(QModelIndex(), row, row);
  m_profiles.removeAt(row);
  endRemoveRows();

  emit countChanged();
  return true;
}

void ProfileListModel::clear() {
  if (m_profiles.isEmpty())
    return;

  for (const ProfilePtr &profile : m_profiles)
    disconnectProfile(profile.data());

  beginResetModel();
  m_profiles.clear();
  endResetModel();

  emit countChanged();
}

QObject *ProfileListModel::at(int row) const {
  const ProfilePtr profile = profileAt(row);
  return profile ? profile.data() : nullptr;
}

QVariantMap ProfileListModel::get(int row) const {
  const ProfilePtr profile = profileAt(row);
  if (!profile)
    return {};

  const QHash<int, QByteArray> names = roleNames();
  QVariantMap result;
  for (auto it = names.cbegin(); it != names.cend(); ++it)
    result.insert(QString::fromLatin1(it.value()), roleData(profile, it.key()));

  return result;
}

int ProfileListModel::indexOf(const QUuid &profileId) const {
  if (profileId.isNull())
    return -1;

  for (int row = 0; row < m_profiles.size(); ++row) {
    const ProfilePtr &profile = m_profiles.at(row);
    if (profile && profile->id() == profileId)
      return row;
  }

  return -1;
}

int ProfileListModel::indexOf(const Profile *profile) const {
  if (!profile)
    return -1;

  for (int row = 0; row < m_profiles.size(); ++row) {
    if (m_profiles.at(row).data() == profile)
      return row;
  }

  return -1;
}

void ProfileListModel::emitRolesChanged(Profile *profile,
                                        const QList<int> &roles) {
  const int row = indexOf(profile);
  if (row < 0)
    return;

  const QModelIndex modelIndex = index(row, 0);
  emit dataChanged(modelIndex, modelIndex, roles);
}

void ProfileListModel::connectProfile(const ProfilePtr &profile) {
  if (!profile)
    return;

  Profile *rawProfile = profile.data();

  connect(rawProfile, &Profile::idChanged, this, [this, rawProfile]() {
    emitRolesChanged(rawProfile, {IdRole});
  });
  connect(rawProfile, &Profile::firstNameChanged, this, [this, rawProfile]() {
    emitRolesChanged(rawProfile, {FirstNameRole});
  });
  connect(rawProfile, &Profile::lastNameChanged, this, [this, rawProfile]() {
    emitRolesChanged(rawProfile, {LastNameRole, Qt::DisplayRole});
  });
  connect(rawProfile, &Profile::accountsChanged, this, [this, rawProfile]() {
    emitRolesChanged(rawProfile, {AccountsRole});
  });
}

void ProfileListModel::disconnectProfile(Profile *profile) {
  if (!profile)
    return;

  disconnect(profile, nullptr, this, nullptr);
}

QVariant ProfileListModel::roleData(const ProfilePtr &profile, int role) const {
  if (!profile)
    return {};

  switch (role) {
  case Qt::DisplayRole:
  case LastNameRole:
  case Qt::EditRole:
    return profile->lastName();
  case ProfileRole:
    return QVariant::fromValue(static_cast<QObject *>(profile.data()));
  case IdRole:
    return profile->id();
  case FirstNameRole:
    return profile->firstName();
  case AccountsRole:
    return QVariant::fromValue(profile->accounts());
  default:
    break;
  }

  return {};
}
