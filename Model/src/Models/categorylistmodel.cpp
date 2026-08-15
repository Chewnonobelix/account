#include "Model/Models/categorylistmodel.h"

#include <QSet>
#include <QString>

namespace {

template <typename Enum> bool enumFromVariant(const QVariant &value, Enum *out) {
  if (!out)
    return false;

  if (value.canConvert<Enum>()) {
    *out = value.value<Enum>();
    return true;
  }

  bool ok = false;
  const int rawValue = value.toInt(&ok);
  if (!ok)
    return false;

  *out = static_cast<Enum>(rawValue);
  return true;
}

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

CategoryListModel::CategoryListModel(QObject *parent)
    : QAbstractListModel(parent) {}

int CategoryListModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;

  return m_categories.size();
}

QVariant CategoryListModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_categories.size())
    return {};

  return roleData(m_categories.at(index.row()), role);
}

bool CategoryListModel::setData(const QModelIndex &index, const QVariant &value,
                                int role) {
  const CategoryPtr category = categoryAt(index.row());
  if (!category)
    return false;

  switch (role) {
  case IdRole: {
    QUuid id;
    if (!uuidFromVariant(value, &id))
      return false;
    category->setId(id);
    return true;
  }
  case NameRole:
  case Qt::EditRole:
    category->setName(value.toString());
    return true;
  case DirectionRole: {
    OpenAccountEnums::Movement direction;
    if (!enumFromVariant(value, &direction))
      return false;
    category->setDirection(direction);
    return true;
  }
  default:
    break;
  }

  return false;
}

Qt::ItemFlags CategoryListModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QHash<int, QByteArray> CategoryListModel::roleNames() const {
  return {{CategoryRole, "category"},
          {IdRole, "id"},
          {NameRole, "name"},
          {DirectionRole, "direction"}};
}

CategoryPtr CategoryListModel::categoryAt(int row) const {
  if (row < 0 || row >= m_categories.size())
    return {};

  return m_categories.at(row);
}

bool CategoryListModel::contains(const QUuid &categoryId) const {
  return indexOf(categoryId) >= 0;
}

void CategoryListModel::setCategories(const QList<CategoryPtr> &categories) {
  QList<CategoryPtr> nextCategories;
  nextCategories.reserve(categories.size());

  // Hash-set dedup instead of an O(n) scan per candidate: with a large
  // bulk-loaded list the scan-per-candidate version turns into an O(n^2)
  // pass that can visibly freeze the UI thread this runs on.
  QSet<const Category *> seenPointers;
  QSet<QUuid> seenIds;
  seenPointers.reserve(categories.size());
  seenIds.reserve(categories.size());

  for (const CategoryPtr &category : categories) {
    if (!category || seenPointers.contains(category.data()))
      continue;
    if (!category->id().isNull() && seenIds.contains(category->id()))
      continue;

    seenPointers.insert(category.data());
    if (!category->id().isNull())
      seenIds.insert(category->id());

    nextCategories.append(category);
  }

  const int oldCount = m_categories.size();

  for (const CategoryPtr &category : m_categories)
    disconnectCategory(category.data());

  beginResetModel();
  m_categories = nextCategories;
  endResetModel();

  for (const CategoryPtr &category : m_categories)
    connectCategory(category);

  if (oldCount != m_categories.size())
    emit countChanged();
}

bool CategoryListModel::addCategory(const CategoryPtr &category) {
  if (!category || indexOf(category.data()) >= 0 ||
      (!category->id().isNull() && contains(category->id()))) {
    return false;
  }

  const int row = m_categories.size();
  beginInsertRows(QModelIndex(), row, row);
  m_categories.append(category);
  connectCategory(category);
  endInsertRows();

  emit countChanged();
  return true;
}

bool CategoryListModel::addCategory(QObject *category) {
  Category *rawCategory = qobject_cast<Category *>(category);
  if (!rawCategory)
    return false;

  return addCategory(CategoryPtr(rawCategory, [](Category *) {}));
}

bool CategoryListModel::removeCategory(const QUuid &categoryId) {
  return removeCategoryAt(indexOf(categoryId));
}

bool CategoryListModel::removeCategoryAt(int row) {
  if (row < 0 || row >= m_categories.size())
    return false;

  disconnectCategory(m_categories.at(row).data());

  beginRemoveRows(QModelIndex(), row, row);
  m_categories.removeAt(row);
  endRemoveRows();

  emit countChanged();
  return true;
}

void CategoryListModel::clear() {
  if (m_categories.isEmpty())
    return;

  for (const CategoryPtr &category : m_categories)
    disconnectCategory(category.data());

  beginResetModel();
  m_categories.clear();
  endResetModel();

  emit countChanged();
}

QObject *CategoryListModel::at(int row) const {
  const CategoryPtr category = categoryAt(row);
  return category ? category.data() : nullptr;
}

QVariantMap CategoryListModel::get(int row) const {
  const CategoryPtr category = categoryAt(row);
  if (!category)
    return {};

  const QHash<int, QByteArray> names = roleNames();
  QVariantMap result;
  for (auto it = names.cbegin(); it != names.cend(); ++it)
    result.insert(QString::fromLatin1(it.value()), roleData(category, it.key()));

  return result;
}

int CategoryListModel::indexOf(const QUuid &categoryId) const {
  if (categoryId.isNull())
    return -1;

  for (int row = 0; row < m_categories.size(); ++row) {
    const CategoryPtr &category = m_categories.at(row);
    if (category && category->id() == categoryId)
      return row;
  }

  return -1;
}

int CategoryListModel::indexOf(const Category *category) const {
  if (!category)
    return -1;

  for (int row = 0; row < m_categories.size(); ++row) {
    if (m_categories.at(row).data() == category)
      return row;
  }

  return -1;
}

void CategoryListModel::emitRolesChanged(Category *category,
                                         const QList<int> &roles) {
  const int row = indexOf(category);
  if (row < 0)
    return;

  const QModelIndex modelIndex = index(row, 0);
  emit dataChanged(modelIndex, modelIndex, roles);
}

void CategoryListModel::connectCategory(const CategoryPtr &category) {
  if (!category)
    return;

  Category *rawCategory = category.data();

  connect(rawCategory, &Category::idChanged, this, [this, rawCategory]() {
    emitRolesChanged(rawCategory, {IdRole});
  });
  connect(rawCategory, &Category::nameChanged, this, [this, rawCategory]() {
    emitRolesChanged(rawCategory, {NameRole, Qt::DisplayRole});
  });
  connect(rawCategory, &Category::directionChanged, this, [this, rawCategory]() {
    emitRolesChanged(rawCategory, {DirectionRole});
  });
}

void CategoryListModel::disconnectCategory(Category *category) {
  if (!category)
    return;

  disconnect(category, nullptr, this, nullptr);
}

QVariant CategoryListModel::roleData(const CategoryPtr &category, int role) const {
  if (!category)
    return {};

  switch (role) {
  case Qt::DisplayRole:
  case NameRole:
  case Qt::EditRole:
    return category->name();
  case CategoryRole:
    return QVariant::fromValue(static_cast<QObject *>(category.data()));
  case IdRole:
    return category->id();
  case DirectionRole:
    return QVariant::fromValue(category->direction());
  default:
    break;
  }

  return {};
}
