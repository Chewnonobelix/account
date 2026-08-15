#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QQmlEngine>
#include <QVariantMap>

#include "account.h"
#include "model_global.h"

class MODEL_EXPORT AccountListModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
  enum Role {
    AccountRole = Qt::UserRole + 1,
    IdRole,
    NumberRole,
    BankRole,
    InterestRole,
    DescriptionRole,
    OpeningRole
  };
  Q_ENUM(Role)

  explicit AccountListModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role) override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QHash<int, QByteArray> roleNames() const override;

  int count() const { return rowCount(); }
  QList<AccountPtr> accounts() const { return m_accounts; }
  AccountPtr accountAt(int row) const;
  bool contains(const QUuid &accountId) const;

  void setAccounts(const QList<AccountPtr> &accounts);
  bool addAccount(const AccountPtr &account);
  // QML can't construct an AccountPtr (QSharedPointer) directly, so this
  // overload wraps a QObject-owned Account (e.g. from Component.createObject)
  // with a no-op deleter: the shared pointer is only used for shared access
  // here, actual lifetime stays with whatever already owns the QObject.
  Q_INVOKABLE bool addAccount(QObject *account);
  bool removeAccount(const QUuid &accountId);
  bool removeAccountAt(int row);
  Q_INVOKABLE void clear();

  Q_INVOKABLE QObject *at(int row) const;
  Q_INVOKABLE QVariantMap get(int row) const;
  Q_INVOKABLE int indexOf(const QUuid &accountId) const;

signals:
  void countChanged();

private:
  int indexOf(const Account *account) const;
  void emitRolesChanged(Account *account, const QList<int> &roles);
  void connectAccount(const AccountPtr &account);
  void disconnectAccount(Account *account);
  QVariant roleData(const AccountPtr &account, int role) const;

private:
  QList<AccountPtr> m_accounts;
};
