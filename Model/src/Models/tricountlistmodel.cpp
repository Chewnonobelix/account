#include "Model/Models/tricountlistmodel.h"

#include <QString>

TricountListModel::TricountListModel(QObject *parent)
    : QAbstractListModel(parent) {}

int TricountListModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;

  return m_tricounts.size();
}

QVariant TricountListModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_tricounts.size())
    return {};

  return roleData(m_tricounts.at(index.row()), role);
}

bool TricountListModel::setData(const QModelIndex &index, const QVariant &value,
                                int role) {
  const TricountPtr tricount = tricountAt(index.row());
  if (!tricount)
    return false;

  switch (role) {
  case IdRole: {
    if (!value.canConvert<QUuid>())
      return false;
    tricount->setId(value.toUuid());
    return true;
  }
  case NameRole:
  case Qt::EditRole:
    tricount->setName(value.toString());
    return true;
  case DescriptionRole:
    tricount->setDescription(value.toString());
    return true;
  case DateRole:
    tricount->setDate(value.toDate());
    return true;
  case CurrencyRole:
    tricount->setCurrency(value.toString());
    return true;
  default:
    break;
  }

  return false;
}

Qt::ItemFlags TricountListModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QHash<int, QByteArray> TricountListModel::roleNames() const {
  return {{TricountRole, "tricount"},
          {IdRole, "id"},
          {NameRole, "name"},
          {DescriptionRole, "description"},
          {DateRole, "date"},
          {CurrencyRole, "currency"},
          {MembersRole, "members"},
          {ExpensesRole, "expenses"}};
}

TricountPtr TricountListModel::tricountAt(int row) const {
  if (row < 0 || row >= m_tricounts.size())
    return {};

  return m_tricounts.at(row);
}

bool TricountListModel::contains(const QUuid &tricountId) const {
  return indexOf(tricountId) >= 0;
}

void TricountListModel::setTricounts(const QList<TricountPtr> &tricounts) {
  QList<TricountPtr> nextTricounts;
  nextTricounts.reserve(tricounts.size());

  auto alreadyAdded = [&nextTricounts](const TricountPtr &candidate) {
    for (const TricountPtr &existing : nextTricounts) {
      if (existing == candidate)
        return true;
      if (existing && candidate && !existing->id().isNull() &&
          existing->id() == candidate->id()) {
        return true;
      }
    }
    return false;
  };

  for (const TricountPtr &tricount : tricounts) {
    if (!tricount || alreadyAdded(tricount))
      continue;

    nextTricounts.append(tricount);
  }

  const int oldCount = m_tricounts.size();

  for (const TricountPtr &tricount : m_tricounts)
    disconnectTricount(tricount.data());

  beginResetModel();
  m_tricounts = nextTricounts;
  endResetModel();

  for (const TricountPtr &tricount : m_tricounts)
    connectTricount(tricount);

  if (oldCount != m_tricounts.size())
    emit countChanged();
}

bool TricountListModel::addTricount(const TricountPtr &tricount) {
  if (!tricount || indexOf(tricount.data()) >= 0 ||
      (!tricount->id().isNull() && contains(tricount->id()))) {
    return false;
  }

  const int row = m_tricounts.size();
  beginInsertRows(QModelIndex(), row, row);
  m_tricounts.append(tricount);
  connectTricount(tricount);
  endInsertRows();

  emit countChanged();
  return true;
}

bool TricountListModel::addTricount(QObject *tricount) {
  Tricount *rawTricount = qobject_cast<Tricount *>(tricount);
  if (!rawTricount)
    return false;

  return addTricount(TricountPtr(rawTricount, [](Tricount *) {}));
}

bool TricountListModel::removeTricount(const QUuid &tricountId) {
  return removeTricountAt(indexOf(tricountId));
}

bool TricountListModel::removeTricountAt(int row) {
  if (row < 0 || row >= m_tricounts.size())
    return false;

  disconnectTricount(m_tricounts.at(row).data());

  beginRemoveRows(QModelIndex(), row, row);
  m_tricounts.removeAt(row);
  endRemoveRows();

  emit countChanged();
  return true;
}

void TricountListModel::clear() {
  if (m_tricounts.isEmpty())
    return;

  for (const TricountPtr &tricount : m_tricounts)
    disconnectTricount(tricount.data());

  beginResetModel();
  m_tricounts.clear();
  endResetModel();

  emit countChanged();
}

QObject *TricountListModel::at(int row) const {
  const TricountPtr tricount = tricountAt(row);
  return tricount ? tricount.data() : nullptr;
}

QVariantMap TricountListModel::get(int row) const {
  const TricountPtr tricount = tricountAt(row);
  if (!tricount)
    return {};

  const QHash<int, QByteArray> names = roleNames();
  QVariantMap result;
  for (auto it = names.cbegin(); it != names.cend(); ++it)
    result.insert(QString::fromLatin1(it.value()), roleData(tricount, it.key()));

  return result;
}

int TricountListModel::indexOf(const QUuid &tricountId) const {
  if (tricountId.isNull())
    return -1;

  for (int row = 0; row < m_tricounts.size(); ++row) {
    const TricountPtr &tricount = m_tricounts.at(row);
    if (tricount && tricount->id() == tricountId)
      return row;
  }

  return -1;
}

int TricountListModel::indexOf(const Tricount *tricount) const {
  if (!tricount)
    return -1;

  for (int row = 0; row < m_tricounts.size(); ++row) {
    if (m_tricounts.at(row).data() == tricount)
      return row;
  }

  return -1;
}

void TricountListModel::emitRolesChanged(Tricount *tricount,
                                         const QList<int> &roles) {
  const int row = indexOf(tricount);
  if (row < 0)
    return;

  const QModelIndex modelIndex = index(row, 0);
  emit dataChanged(modelIndex, modelIndex, roles);
}

void TricountListModel::connectTricount(const TricountPtr &tricount) {
  if (!tricount)
    return;

  Tricount *rawTricount = tricount.data();

  connect(rawTricount, &Tricount::idChanged, this, [this, rawTricount]() {
    emitRolesChanged(rawTricount, {IdRole});
  });
  connect(rawTricount, &Tricount::nameChanged, this, [this, rawTricount]() {
    emitRolesChanged(rawTricount, {NameRole, Qt::DisplayRole});
  });
  connect(rawTricount, &Tricount::descriptionChanged, this, [this, rawTricount]() {
    emitRolesChanged(rawTricount, {DescriptionRole});
  });
  connect(rawTricount, &Tricount::dateChanged, this, [this, rawTricount]() {
    emitRolesChanged(rawTricount, {DateRole});
  });
  connect(rawTricount, &Tricount::currencyChanged, this, [this, rawTricount]() {
    emitRolesChanged(rawTricount, {CurrencyRole});
  });
  connect(rawTricount, &Tricount::membersChanged, this, [this, rawTricount]() {
    emitRolesChanged(rawTricount, {MembersRole});
  });
  connect(rawTricount, &Tricount::expensesChanged, this, [this, rawTricount]() {
    emitRolesChanged(rawTricount, {ExpensesRole});
  });
}

void TricountListModel::disconnectTricount(Tricount *tricount) {
  if (!tricount)
    return;

  disconnect(tricount, nullptr, this, nullptr);
}

QVariant TricountListModel::roleData(const TricountPtr &tricount, int role) const {
  if (!tricount)
    return {};

  switch (role) {
  case Qt::DisplayRole:
  case NameRole:
  case Qt::EditRole:
    return tricount->name();
  case TricountRole:
    return QVariant::fromValue(static_cast<QObject *>(tricount.data()));
  case IdRole:
    return tricount->id();
  case DescriptionRole:
    return tricount->description();
  case DateRole:
    return tricount->date();
  case CurrencyRole:
    return tricount->currency();
  case MembersRole:
    return QVariant::fromValue(tricount->members());
  case ExpensesRole:
    return QVariant::fromValue(tricount->expenses());
  default:
    break;
  }

  return {};
}
