#include "Model/Models/accountlistmodel.h"

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

AccountListModel::AccountListModel(QObject *parent)
    : QAbstractListModel(parent) {}

int AccountListModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return 0;

  return m_accounts.size();
}

QVariant AccountListModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || index.row() < 0 || index.row() >= m_accounts.size())
    return {};

  return roleData(m_accounts.at(index.row()), role);
}

bool AccountListModel::setData(const QModelIndex &index, const QVariant &value,
                               int role) {
  const AccountPtr account = accountAt(index.row());
  if (!account)
    return false;

  switch (role) {
  case IdRole: {
    QUuid id;
    if (!uuidFromVariant(value, &id))
      return false;
    account->setId(id);
    return true;
  }
  case NumberRole:
  case Qt::EditRole:
    account->setNumber(value.toString());
    return true;
  case BankRole:
    account->setBank(value.toString());
    return true;
  case InterestRole: {
    bool ok = false;
    const double interest = value.toDouble(&ok);
    if (!ok)
      return false;
    account->setInterest(interest);
    return true;
  }
  case DescriptionRole:
    account->setDescription(value.toString());
    return true;
  case OpeningRole:
    account->setOpening(value.toDate());
    return true;
  default:
    break;
  }

  return false;
}

Qt::ItemFlags AccountListModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QHash<int, QByteArray> AccountListModel::roleNames() const {
  return {{AccountRole, "account"},
          {IdRole, "id"},
          {NumberRole, "number"},
          {BankRole, "bank"},
          {InterestRole, "interest"},
          {DescriptionRole, "description"},
          {OpeningRole, "opening"}};
}

AccountPtr AccountListModel::accountAt(int row) const {
  if (row < 0 || row >= m_accounts.size())
    return {};

  return m_accounts.at(row);
}

bool AccountListModel::contains(const QUuid &accountId) const {
  return indexOf(accountId) >= 0;
}

void AccountListModel::setAccounts(const QList<AccountPtr> &accounts) {
  QList<AccountPtr> nextAccounts;
  nextAccounts.reserve(accounts.size());

  auto alreadyAdded = [&nextAccounts](const AccountPtr &candidate) {
    for (const AccountPtr &existing : nextAccounts) {
      if (existing == candidate)
        return true;
      if (existing && candidate && !existing->id().isNull() &&
          existing->id() == candidate->id()) {
        return true;
      }
    }
    return false;
  };

  for (const AccountPtr &account : accounts) {
    if (!account || alreadyAdded(account))
      continue;

    nextAccounts.append(account);
  }

  const int oldCount = m_accounts.size();

  for (const AccountPtr &account : m_accounts)
    disconnectAccount(account.data());

  beginResetModel();
  m_accounts = nextAccounts;
  endResetModel();

  for (const AccountPtr &account : m_accounts)
    connectAccount(account);

  if (oldCount != m_accounts.size())
    emit countChanged();
}

bool AccountListModel::addAccount(const AccountPtr &account) {
  if (!account || indexOf(account.data()) >= 0 ||
      (!account->id().isNull() && contains(account->id()))) {
    return false;
  }

  const int row = m_accounts.size();
  beginInsertRows(QModelIndex(), row, row);
  m_accounts.append(account);
  connectAccount(account);
  endInsertRows();

  emit countChanged();
  return true;
}

bool AccountListModel::addAccount(QObject *account) {
  Account *rawAccount = qobject_cast<Account *>(account);
  if (!rawAccount)
    return false;

  return addAccount(AccountPtr(rawAccount, [](Account *) {}));
}

bool AccountListModel::removeAccount(const QUuid &accountId) {
  return removeAccountAt(indexOf(accountId));
}

bool AccountListModel::removeAccountAt(int row) {
  if (row < 0 || row >= m_accounts.size())
    return false;

  disconnectAccount(m_accounts.at(row).data());

  beginRemoveRows(QModelIndex(), row, row);
  m_accounts.removeAt(row);
  endRemoveRows();

  emit countChanged();
  return true;
}

void AccountListModel::clear() {
  if (m_accounts.isEmpty())
    return;

  for (const AccountPtr &account : m_accounts)
    disconnectAccount(account.data());

  beginResetModel();
  m_accounts.clear();
  endResetModel();

  emit countChanged();
}

QObject *AccountListModel::at(int row) const {
  const AccountPtr account = accountAt(row);
  return account ? account.data() : nullptr;
}

QVariantMap AccountListModel::get(int row) const {
  const AccountPtr account = accountAt(row);
  if (!account)
    return {};

  const QHash<int, QByteArray> names = roleNames();
  QVariantMap result;
  for (auto it = names.cbegin(); it != names.cend(); ++it)
    result.insert(QString::fromLatin1(it.value()), roleData(account, it.key()));

  return result;
}

int AccountListModel::indexOf(const QUuid &accountId) const {
  if (accountId.isNull())
    return -1;

  for (int row = 0; row < m_accounts.size(); ++row) {
    const AccountPtr &account = m_accounts.at(row);
    if (account && account->id() == accountId)
      return row;
  }

  return -1;
}

int AccountListModel::indexOf(const Account *account) const {
  if (!account)
    return -1;

  for (int row = 0; row < m_accounts.size(); ++row) {
    if (m_accounts.at(row).data() == account)
      return row;
  }

  return -1;
}

void AccountListModel::emitRolesChanged(Account *account,
                                        const QList<int> &roles) {
  const int row = indexOf(account);
  if (row < 0)
    return;

  const QModelIndex modelIndex = index(row, 0);
  emit dataChanged(modelIndex, modelIndex, roles);
}

void AccountListModel::connectAccount(const AccountPtr &account) {
  if (!account)
    return;

  Account *rawAccount = account.data();

  connect(rawAccount, &Account::idChanged, this, [this, rawAccount]() {
    emitRolesChanged(rawAccount, {IdRole});
  });
  connect(rawAccount, &Account::numberChanged, this, [this, rawAccount]() {
    emitRolesChanged(rawAccount, {NumberRole, Qt::DisplayRole});
  });
  connect(rawAccount, &Account::bankChanged, this, [this, rawAccount]() {
    emitRolesChanged(rawAccount, {BankRole});
  });
  connect(rawAccount, &Account::interestChanged, this, [this, rawAccount]() {
    emitRolesChanged(rawAccount, {InterestRole});
  });
  connect(rawAccount, &Account::descriptionChanged, this, [this, rawAccount]() {
    emitRolesChanged(rawAccount, {DescriptionRole});
  });
  connect(rawAccount, &Account::openingChanged, this, [this, rawAccount]() {
    emitRolesChanged(rawAccount, {OpeningRole});
  });
}

void AccountListModel::disconnectAccount(Account *account) {
  if (!account)
    return;

  disconnect(account, nullptr, this, nullptr);
}

QVariant AccountListModel::roleData(const AccountPtr &account, int role) const {
  if (!account)
    return {};

  switch (role) {
  case Qt::DisplayRole:
  case NumberRole:
  case Qt::EditRole:
    return account->number();
  case AccountRole:
    return QVariant::fromValue(static_cast<QObject *>(account.data()));
  case IdRole:
    return account->id();
  case BankRole:
    return account->bank();
  case InterestRole:
    return account->interest();
  case DescriptionRole:
    return account->description();
  case OpeningRole:
    return account->opening();
  default:
    break;
  }

  return {};
}
