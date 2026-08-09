#include "Model/Models/budgetlistmodel.h"

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

} // namespace

BudgetListModel::BudgetListModel(QObject *parent)
    : QAbstractListModel(parent) {}

int BudgetListModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;

  return m_budgets.size();
}

QVariant BudgetListModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_budgets.size())
    return {};

  return roleData(m_budgets.at(index.row()), role);
}

bool BudgetListModel::setData(const QModelIndex &index, const QVariant &value,
                              int role) {
  const BudgetPtr budget = budgetAt(index.row());
  if (!budget)
    return false;

  switch (role) {
  case IdRole: {
    QUuid id;
    if (!uuidFromVariant(value, &id))
      return false;
    budget->setId(id);
    return true;
  }
  case NameRole:
  case Qt::EditRole:
    budget->setName(value.toString());
    return true;
  case CategoryIdRole: {
    QUuid categoryId;
    if (!uuidFromVariant(value, &categoryId))
      return false;
    budget->setCategoryId(categoryId);
    return true;
  }
  case ThresholdRole: {
    bool ok = false;
    const double threshold = value.toDouble(&ok);
    if (!ok)
      return false;
    budget->setThreshold(threshold);
    return true;
  }
  case StartDateRole:
    budget->setStartDate(value.toDate());
    return true;
  case EndDateRole:
    budget->setEndDate(value.toDate());
    return true;
  case RecurrenceRole: {
    OpenAccountEnums::Frequency recurrence;
    if (!enumFromVariant(value, &recurrence))
      return false;
    budget->setRecurrence(recurrence);
    return true;
  }
  default:
    break;
  }

  return false;
}

Qt::ItemFlags BudgetListModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QHash<int, QByteArray> BudgetListModel::roleNames() const {
  return {{BudgetRole, "budget"},
          {IdRole, "id"},
          {NameRole, "name"},
          {CategoryIdRole, "categoryId"},
          {ThresholdRole, "threshold"},
          {StartDateRole, "startDate"},
          {EndDateRole, "endDate"},
          {RecurrenceRole, "recurrence"}};
}

BudgetPtr BudgetListModel::budgetAt(int row) const {
  if (row < 0 || row >= m_budgets.size())
    return {};

  return m_budgets.at(row);
}

bool BudgetListModel::contains(const QUuid &budgetId) const {
  return indexOf(budgetId) >= 0;
}

void BudgetListModel::setBudgets(const QList<BudgetPtr> &budgets) {
  QList<BudgetPtr> nextBudgets;
  nextBudgets.reserve(budgets.size());

  auto alreadyAdded = [&nextBudgets](const BudgetPtr &candidate) {
    for (const BudgetPtr &existing : nextBudgets) {
      if (existing == candidate)
        return true;
      if (existing && candidate && !existing->id().isNull() &&
          existing->id() == candidate->id()) {
        return true;
      }
    }
    return false;
  };

  for (const BudgetPtr &budget : budgets) {
    if (!budget || alreadyAdded(budget))
      continue;

    nextBudgets.append(budget);
  }

  const int oldCount = m_budgets.size();

  for (const BudgetPtr &budget : m_budgets)
    disconnectBudget(budget.data());

  beginResetModel();
  m_budgets = nextBudgets;
  endResetModel();

  for (const BudgetPtr &budget : m_budgets)
    connectBudget(budget);

  if (oldCount != m_budgets.size())
    emit countChanged();
}

bool BudgetListModel::addBudget(const BudgetPtr &budget) {
  if (!budget || indexOf(budget.data()) >= 0 ||
      (!budget->id().isNull() && contains(budget->id()))) {
    return false;
  }

  const int row = m_budgets.size();
  beginInsertRows(QModelIndex(), row, row);
  m_budgets.append(budget);
  connectBudget(budget);
  endInsertRows();

  emit countChanged();
  return true;
}

bool BudgetListModel::addBudget(QObject *budget) {
  Budget *rawBudget = qobject_cast<Budget *>(budget);
  if (!rawBudget)
    return false;

  return addBudget(BudgetPtr(rawBudget, [](Budget *) {}));
}

bool BudgetListModel::removeBudget(const QUuid &budgetId) {
  return removeBudgetAt(indexOf(budgetId));
}

bool BudgetListModel::removeBudgetAt(int row) {
  if (row < 0 || row >= m_budgets.size())
    return false;

  disconnectBudget(m_budgets.at(row).data());

  beginRemoveRows(QModelIndex(), row, row);
  m_budgets.removeAt(row);
  endRemoveRows();

  emit countChanged();
  return true;
}

void BudgetListModel::clear() {
  if (m_budgets.isEmpty())
    return;

  for (const BudgetPtr &budget : m_budgets)
    disconnectBudget(budget.data());

  beginResetModel();
  m_budgets.clear();
  endResetModel();

  emit countChanged();
}

QObject *BudgetListModel::at(int row) const {
  const BudgetPtr budget = budgetAt(row);
  return budget ? budget.data() : nullptr;
}

QVariantMap BudgetListModel::get(int row) const {
  const BudgetPtr budget = budgetAt(row);
  if (!budget)
    return {};

  const QHash<int, QByteArray> names = roleNames();
  QVariantMap result;
  for (auto it = names.cbegin(); it != names.cend(); ++it)
    result.insert(QString::fromLatin1(it.value()), roleData(budget, it.key()));

  return result;
}

int BudgetListModel::indexOf(const QUuid &budgetId) const {
  if (budgetId.isNull())
    return -1;

  for (int row = 0; row < m_budgets.size(); ++row) {
    const BudgetPtr &budget = m_budgets.at(row);
    if (budget && budget->id() == budgetId)
      return row;
  }

  return -1;
}

int BudgetListModel::indexOf(const Budget *budget) const {
  if (!budget)
    return -1;

  for (int row = 0; row < m_budgets.size(); ++row) {
    if (m_budgets.at(row).data() == budget)
      return row;
  }

  return -1;
}

void BudgetListModel::emitRolesChanged(Budget *budget,
                                       const QList<int> &roles) {
  const int row = indexOf(budget);
  if (row < 0)
    return;

  const QModelIndex modelIndex = index(row, 0);
  emit dataChanged(modelIndex, modelIndex, roles);
}

void BudgetListModel::connectBudget(const BudgetPtr &budget) {
  if (!budget)
    return;

  Budget *rawBudget = budget.data();

  connect(rawBudget, &Budget::idChanged, this, [this, rawBudget]() {
    emitRolesChanged(rawBudget, {IdRole});
  });
  connect(rawBudget, &Budget::nameChanged, this, [this, rawBudget]() {
    emitRolesChanged(rawBudget, {NameRole, Qt::DisplayRole});
  });
  connect(rawBudget, &Budget::categoryIdChanged, this, [this, rawBudget]() {
    emitRolesChanged(rawBudget, {CategoryIdRole});
  });
  connect(rawBudget, &Budget::thresholdChanged, this, [this, rawBudget]() {
    emitRolesChanged(rawBudget, {ThresholdRole});
  });
  connect(rawBudget, &Budget::startDateChanged, this, [this, rawBudget]() {
    emitRolesChanged(rawBudget, {StartDateRole});
  });
  connect(rawBudget, &Budget::endDateChanged, this, [this, rawBudget]() {
    emitRolesChanged(rawBudget, {EndDateRole});
  });
  connect(rawBudget, &Budget::recurrenceChanged, this, [this, rawBudget]() {
    emitRolesChanged(rawBudget, {RecurrenceRole});
  });
}

void BudgetListModel::disconnectBudget(Budget *budget) {
  if (!budget)
    return;

  disconnect(budget, nullptr, this, nullptr);
}

QVariant BudgetListModel::roleData(const BudgetPtr &budget, int role) const {
  if (!budget)
    return {};

  switch (role) {
  case Qt::DisplayRole:
  case NameRole:
  case Qt::EditRole:
    return budget->name();
  case BudgetRole:
    return QVariant::fromValue(static_cast<QObject *>(budget.data()));
  case IdRole:
    return budget->id();
  case CategoryIdRole:
    return budget->categoryId();
  case ThresholdRole:
    return budget->threshold();
  case StartDateRole:
    return budget->startDate();
  case EndDateRole:
    return budget->endDate();
  case RecurrenceRole:
    return QVariant::fromValue(budget->recurrence());
  default:
    break;
  }

  return {};
}
