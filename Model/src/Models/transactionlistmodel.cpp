#include "Model/Models/transactionlistmodel.h"

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

TransactionListModel::TransactionListModel(QObject *parent)
    : QAbstractListModel(parent) {}

int TransactionListModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;

  return m_transactions.size();
}

QVariant TransactionListModel::data(const QModelIndex &index,
                                    int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_transactions.size())
    return {};

  return roleData(m_transactions.at(index.row()), role);
}

bool TransactionListModel::setData(const QModelIndex &index,
                                   const QVariant &value, int role) {
  const TransactionPtr transaction = transactionAt(index.row());
  if (!transaction)
    return false;

  switch (role) {
  case IdRole: {
    QUuid id;
    if (!uuidFromVariant(value, &id))
      return false;
    transaction->setId(id);
    return true;
  }
  case ValueRole:
    transaction->setValue(value.toDouble());
    return true;
  case DescriptionRole:
  case Qt::EditRole:
    transaction->setDescription(value.toString());
    return true;
  case SupportRole: {
    OpenAccountEnums::Support support;
    if (!enumFromVariant(value, &support))
      return false;
    transaction->setSupport(support);
    return true;
  }
  case DateRole: {
    const QDate date = value.canConvert<QDate>()
                           ? value.toDate()
                           : QDate::fromString(value.toString(), Qt::ISODate);
    if (!date.isValid())
      return false;
    transaction->setDate(date);
    return true;
  }
  case MovementRole: {
    OpenAccountEnums::Movement movement;
    if (!enumFromVariant(value, &movement))
      return false;
    transaction->setMovement(movement);
    return true;
  }
  case IsVisibleRole:
    transaction->setIsVisible(value.toBool());
    return true;
  case AccountIdRole: {
    QUuid accountId;
    if (!uuidFromVariant(value, &accountId))
      return false;
    transaction->setAccountId(accountId);
    return true;
  }
  case CategoryRole: {
    QUuid categoryId;
    if (!uuidFromVariant(value, &categoryId))
      return false;
    transaction->setCategory(categoryId);
    return true;
  }
  default:
    break;
  }

  return false;
}

Qt::ItemFlags TransactionListModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QHash<int, QByteArray> TransactionListModel::roleNames() const {
  return {{TransactionRole, "transaction"},
          {IdRole, "id"},
          {ValueRole, "value"},
          {DescriptionRole, "description"},
          {SupportRole, "support"},
          {DateRole, "date"},
          {MovementRole, "movement"},
          {IsVisibleRole, "isVisible"},
          {AccountIdRole, "accountId"},
          {CategoryRole, "category"}};
}

TransactionPtr TransactionListModel::transactionAt(int row) const {
  if (row < 0 || row >= m_transactions.size())
    return {};

  return m_transactions.at(row);
}

bool TransactionListModel::contains(const QUuid &transactionId) const {
  return indexOf(transactionId) >= 0;
}

void TransactionListModel::setTransactions(
    const QList<TransactionPtr> &transactions) {
  QList<TransactionPtr> nextTransactions;
  nextTransactions.reserve(transactions.size());

  auto alreadyAdded = [&nextTransactions](const TransactionPtr &candidate) {
    for (const TransactionPtr &existing : nextTransactions) {
      if (existing == candidate)
        return true;
      if (existing && candidate && !existing->id().isNull() &&
          existing->id() == candidate->id()) {
        return true;
      }
    }
    return false;
  };

  for (const TransactionPtr &transaction : transactions) {
    if (!transaction || alreadyAdded(transaction))
      continue;

    nextTransactions.append(transaction);
  }

  const int oldCount = m_transactions.size();

  for (const TransactionPtr &transaction : m_transactions)
    disconnectTransaction(transaction.data());

  beginResetModel();
  m_transactions = nextTransactions;
  endResetModel();

  for (const TransactionPtr &transaction : m_transactions)
    connectTransaction(transaction);

  if (oldCount != m_transactions.size())
    emit countChanged();
}

bool TransactionListModel::addTransaction(const TransactionPtr &transaction) {
  if (!transaction || indexOf(transaction.data()) >= 0 ||
      (!transaction->id().isNull() && contains(transaction->id()))) {
    return false;
  }

  const int row = m_transactions.size();
  beginInsertRows(QModelIndex(), row, row);
  m_transactions.append(transaction);
  connectTransaction(transaction);
  endInsertRows();

  emit countChanged();
  return true;
}

bool TransactionListModel::removeTransaction(const QUuid &transactionId) {
  return removeTransactionAt(indexOf(transactionId));
}

bool TransactionListModel::removeTransactionAt(int row) {
  if (row < 0 || row >= m_transactions.size())
    return false;

  disconnectTransaction(m_transactions.at(row).data());

  beginRemoveRows(QModelIndex(), row, row);
  m_transactions.removeAt(row);
  endRemoveRows();

  emit countChanged();
  return true;
}

void TransactionListModel::clear() {
  if (m_transactions.isEmpty())
    return;

  for (const TransactionPtr &transaction : m_transactions)
    disconnectTransaction(transaction.data());

  beginResetModel();
  m_transactions.clear();
  endResetModel();

  emit countChanged();
}

QObject *TransactionListModel::at(int row) const {
  const TransactionPtr transaction = transactionAt(row);
  return transaction ? transaction.data() : nullptr;
}

QVariantMap TransactionListModel::get(int row) const {
  const TransactionPtr transaction = transactionAt(row);
  if (!transaction)
    return {};

  const QHash<int, QByteArray> names = roleNames();
  QVariantMap result;
  for (auto it = names.cbegin(); it != names.cend(); ++it)
    result.insert(QString::fromLatin1(it.value()), roleData(transaction, it.key()));

  return result;
}

int TransactionListModel::indexOf(const QUuid &transactionId) const {
  if (transactionId.isNull())
    return -1;

  for (int row = 0; row < m_transactions.size(); ++row) {
    const TransactionPtr &transaction = m_transactions.at(row);
    if (transaction && transaction->id() == transactionId)
      return row;
  }

  return -1;
}

int TransactionListModel::indexOf(const Transaction *transaction) const {
  if (!transaction)
    return -1;

  for (int row = 0; row < m_transactions.size(); ++row) {
    if (m_transactions.at(row).data() == transaction)
      return row;
  }

  return -1;
}

void TransactionListModel::emitRolesChanged(Transaction *transaction,
                                            const QList<int> &roles) {
  const int row = indexOf(transaction);
  if (row < 0)
    return;

  const QModelIndex modelIndex = index(row, 0);
  emit dataChanged(modelIndex, modelIndex, roles);
}

void TransactionListModel::connectTransaction(const TransactionPtr &transaction) {
  if (!transaction)
    return;

  Transaction *rawTransaction = transaction.data();

  connect(rawTransaction, &Transaction::idChanged, this, [this, rawTransaction]() {
    emitRolesChanged(rawTransaction, {IdRole});
  });
  connect(rawTransaction, &Transaction::valueChanged, this,
          [this, rawTransaction]() {
            emitRolesChanged(rawTransaction, {ValueRole});
          });
  connect(rawTransaction, &Transaction::descriptionChanged, this,
          [this, rawTransaction]() {
            emitRolesChanged(rawTransaction, {DescriptionRole, Qt::DisplayRole});
          });
  connect(rawTransaction, &Transaction::supportChanged, this,
          [this, rawTransaction]() {
            emitRolesChanged(rawTransaction, {SupportRole});
          });
  connect(rawTransaction, &Transaction::dateChanged, this,
          [this, rawTransaction]() {
            emitRolesChanged(rawTransaction, {DateRole});
          });
  connect(rawTransaction, &Transaction::movementChanged, this,
          [this, rawTransaction]() {
            emitRolesChanged(rawTransaction, {MovementRole});
          });
  connect(rawTransaction, &Transaction::isVisibleChanged, this,
          [this, rawTransaction]() {
            emitRolesChanged(rawTransaction, {IsVisibleRole});
          });
  connect(rawTransaction, &Transaction::accountIdChanged, this,
          [this, rawTransaction]() {
            emitRolesChanged(rawTransaction, {AccountIdRole});
          });
  connect(rawTransaction, &Transaction::categoryChanged, this,
          [this, rawTransaction]() {
            emitRolesChanged(rawTransaction, {CategoryRole});
          });
}

void TransactionListModel::disconnectTransaction(Transaction *transaction) {
  if (!transaction)
    return;

  disconnect(transaction, nullptr, this, nullptr);
}

QVariant TransactionListModel::roleData(const TransactionPtr &transaction,
                                        int role) const {
  if (!transaction)
    return {};

  switch (role) {
  case Qt::DisplayRole:
  case DescriptionRole:
  case Qt::EditRole:
    return transaction->description();
  case TransactionRole:
    return QVariant::fromValue(static_cast<QObject *>(transaction.data()));
  case IdRole:
    return transaction->id();
  case ValueRole:
    return transaction->value();
  case SupportRole:
    return QVariant::fromValue(transaction->support());
  case DateRole:
    return transaction->date();
  case MovementRole:
    return QVariant::fromValue(transaction->movement());
  case IsVisibleRole:
    return transaction->isVisible();
  case AccountIdRole:
    return transaction->accountId();
  case CategoryRole:
    return transaction->category();
  default:
    break;
  }

  return {};
}
