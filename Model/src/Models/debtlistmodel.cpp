#include "Model/Models/debtlistmodel.h"

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

DebtListModel::DebtListModel(QObject *parent) : QAbstractListModel(parent) {}

int DebtListModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;

  return m_debts.size();
}

QVariant DebtListModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_debts.size())
    return {};

  return roleData(m_debts.at(index.row()), role);
}

bool DebtListModel::setData(const QModelIndex &index, const QVariant &value,
                            int role) {
  const DebtPtr debt = debtAt(index.row());
  if (!debt)
    return false;

  switch (role) {
  case IdRole: {
    QUuid id;
    if (!uuidFromVariant(value, &id))
      return false;
    debt->setId(id);
    return true;
  }
  case NameRole:
  case Qt::EditRole:
    debt->setName(value.toString());
    return true;
  case CounterpartyRole:
    debt->setCounterparty(value.toString());
    return true;
  case PrincipalRole: {
    bool ok = false;
    const double principal = value.toDouble(&ok);
    if (!ok)
      return false;
    debt->setPrincipal(principal);
    return true;
  }
  case RemainingRole: {
    bool ok = false;
    const double remaining = value.toDouble(&ok);
    if (!ok)
      return false;
    debt->setRemaining(remaining);
    return true;
  }
  case InterestRole: {
    bool ok = false;
    const double interest = value.toDouble(&ok);
    if (!ok)
      return false;
    debt->setInterest(interest);
    return true;
  }
  case StartDateRole:
    debt->setStartDate(value.toDate());
    return true;
  case EndDateRole:
    debt->setEndDate(value.toDate());
    return true;
  case RecurrenceRole: {
    OpenAccountEnums::Frequency recurrence;
    if (!enumFromVariant(value, &recurrence))
      return false;
    debt->setRecurrence(recurrence);
    return true;
  }
  case DirectionRole: {
    OpenAccountEnums::Movement direction;
    if (!enumFromVariant(value, &direction))
      return false;
    debt->setDirection(direction);
    return true;
  }
  case DescriptionRole:
    debt->setDescription(value.toString());
    return true;
  case AccountIdRole: {
    QUuid accountId;
    if (!uuidFromVariant(value, &accountId))
      return false;
    debt->setAccountId(accountId);
    return true;
  }
  default:
    break;
  }

  return false;
}

Qt::ItemFlags DebtListModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QHash<int, QByteArray> DebtListModel::roleNames() const {
  return {{DebtRole, "debt"},
          {IdRole, "id"},
          {NameRole, "name"},
          {CounterpartyRole, "counterparty"},
          {PrincipalRole, "principal"},
          {RemainingRole, "remaining"},
          {InterestRole, "interest"},
          {StartDateRole, "startDate"},
          {EndDateRole, "endDate"},
          {RecurrenceRole, "recurrence"},
          {DirectionRole, "direction"},
          {DescriptionRole, "description"},
          {AccountIdRole, "accountId"}};
}

DebtPtr DebtListModel::debtAt(int row) const {
  if (row < 0 || row >= m_debts.size())
    return {};

  return m_debts.at(row);
}

bool DebtListModel::contains(const QUuid &debtId) const {
  return indexOf(debtId) >= 0;
}

void DebtListModel::setDebts(const QList<DebtPtr> &debts) {
  QList<DebtPtr> nextDebts;
  nextDebts.reserve(debts.size());

  auto alreadyAdded = [&nextDebts](const DebtPtr &candidate) {
    for (const DebtPtr &existing : nextDebts) {
      if (existing == candidate)
        return true;
      if (existing && candidate && !existing->id().isNull() &&
          existing->id() == candidate->id()) {
        return true;
      }
    }
    return false;
  };

  for (const DebtPtr &debt : debts) {
    if (!debt || alreadyAdded(debt))
      continue;

    nextDebts.append(debt);
  }

  const int oldCount = m_debts.size();

  for (const DebtPtr &debt : m_debts)
    disconnectDebt(debt.data());

  beginResetModel();
  m_debts = nextDebts;
  endResetModel();

  for (const DebtPtr &debt : m_debts)
    connectDebt(debt);

  if (oldCount != m_debts.size())
    emit countChanged();
}

bool DebtListModel::addDebt(const DebtPtr &debt) {
  if (!debt || indexOf(debt.data()) >= 0 ||
      (!debt->id().isNull() && contains(debt->id()))) {
    return false;
  }

  const int row = m_debts.size();
  beginInsertRows(QModelIndex(), row, row);
  m_debts.append(debt);
  connectDebt(debt);
  endInsertRows();

  emit countChanged();
  return true;
}

bool DebtListModel::addDebt(QObject *debt) {
  Debt *rawDebt = qobject_cast<Debt *>(debt);
  if (!rawDebt)
    return false;

  return addDebt(DebtPtr(rawDebt, [](Debt *) {}));
}

bool DebtListModel::removeDebt(const QUuid &debtId) {
  return removeDebtAt(indexOf(debtId));
}

bool DebtListModel::removeDebtAt(int row) {
  if (row < 0 || row >= m_debts.size())
    return false;

  disconnectDebt(m_debts.at(row).data());

  beginRemoveRows(QModelIndex(), row, row);
  m_debts.removeAt(row);
  endRemoveRows();

  emit countChanged();
  return true;
}

void DebtListModel::clear() {
  if (m_debts.isEmpty())
    return;

  for (const DebtPtr &debt : m_debts)
    disconnectDebt(debt.data());

  beginResetModel();
  m_debts.clear();
  endResetModel();

  emit countChanged();
}

QObject *DebtListModel::at(int row) const {
  const DebtPtr debt = debtAt(row);
  return debt ? debt.data() : nullptr;
}

QVariantMap DebtListModel::get(int row) const {
  const DebtPtr debt = debtAt(row);
  if (!debt)
    return {};

  const QHash<int, QByteArray> names = roleNames();
  QVariantMap result;
  for (auto it = names.cbegin(); it != names.cend(); ++it)
    result.insert(QString::fromLatin1(it.value()), roleData(debt, it.key()));

  return result;
}

int DebtListModel::indexOf(const QUuid &debtId) const {
  if (debtId.isNull())
    return -1;

  for (int row = 0; row < m_debts.size(); ++row) {
    const DebtPtr &debt = m_debts.at(row);
    if (debt && debt->id() == debtId)
      return row;
  }

  return -1;
}

int DebtListModel::indexOf(const Debt *debt) const {
  if (!debt)
    return -1;

  for (int row = 0; row < m_debts.size(); ++row) {
    if (m_debts.at(row).data() == debt)
      return row;
  }

  return -1;
}

void DebtListModel::emitRolesChanged(Debt *debt, const QList<int> &roles) {
  const int row = indexOf(debt);
  if (row < 0)
    return;

  const QModelIndex modelIndex = index(row, 0);
  emit dataChanged(modelIndex, modelIndex, roles);
}

void DebtListModel::connectDebt(const DebtPtr &debt) {
  if (!debt)
    return;

  Debt *rawDebt = debt.data();

  connect(rawDebt, &Debt::idChanged, this, [this, rawDebt]() {
    emitRolesChanged(rawDebt, {IdRole});
  });
  connect(rawDebt, &Debt::nameChanged, this, [this, rawDebt]() {
    emitRolesChanged(rawDebt, {NameRole, Qt::DisplayRole});
  });
  connect(rawDebt, &Debt::counterpartyChanged, this, [this, rawDebt]() {
    emitRolesChanged(rawDebt, {CounterpartyRole});
  });
  connect(rawDebt, &Debt::principalChanged, this, [this, rawDebt]() {
    emitRolesChanged(rawDebt, {PrincipalRole});
  });
  connect(rawDebt, &Debt::remainingChanged, this, [this, rawDebt]() {
    emitRolesChanged(rawDebt, {RemainingRole});
  });
  connect(rawDebt, &Debt::interestChanged, this, [this, rawDebt]() {
    emitRolesChanged(rawDebt, {InterestRole});
  });
  connect(rawDebt, &Debt::startDateChanged, this, [this, rawDebt]() {
    emitRolesChanged(rawDebt, {StartDateRole});
  });
  connect(rawDebt, &Debt::endDateChanged, this, [this, rawDebt]() {
    emitRolesChanged(rawDebt, {EndDateRole});
  });
  connect(rawDebt, &Debt::recurrenceChanged, this, [this, rawDebt]() {
    emitRolesChanged(rawDebt, {RecurrenceRole});
  });
  connect(rawDebt, &Debt::directionChanged, this, [this, rawDebt]() {
    emitRolesChanged(rawDebt, {DirectionRole});
  });
  connect(rawDebt, &Debt::descriptionChanged, this, [this, rawDebt]() {
    emitRolesChanged(rawDebt, {DescriptionRole});
  });
  connect(rawDebt, &Debt::accountIdChanged, this, [this, rawDebt]() {
    emitRolesChanged(rawDebt, {AccountIdRole});
  });
}

void DebtListModel::disconnectDebt(Debt *debt) {
  if (!debt)
    return;

  disconnect(debt, nullptr, this, nullptr);
}

QVariant DebtListModel::roleData(const DebtPtr &debt, int role) const {
  if (!debt)
    return {};

  switch (role) {
  case Qt::DisplayRole:
  case NameRole:
  case Qt::EditRole:
    return debt->name();
  case DebtRole:
    return QVariant::fromValue(static_cast<QObject *>(debt.data()));
  case IdRole:
    return debt->id();
  case CounterpartyRole:
    return debt->counterparty();
  case PrincipalRole:
    return debt->principal();
  case RemainingRole:
    return debt->remaining();
  case InterestRole:
    return debt->interest();
  case StartDateRole:
    return debt->startDate();
  case EndDateRole:
    return debt->endDate();
  case RecurrenceRole:
    return QVariant::fromValue(debt->recurrence());
  case DirectionRole:
    return QVariant::fromValue(debt->direction());
  case DescriptionRole:
    return debt->description();
  case AccountIdRole:
    return debt->accountId();
  default:
    break;
  }

  return {};
}
