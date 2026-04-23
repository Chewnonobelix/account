#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QVariantMap>

#include "model_global.h"
#include "transaction.h"

class MODEL_EXPORT TransactionListModel : public QAbstractListModel {
  Q_OBJECT

  Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
  enum Role {
    TransactionRole = Qt::UserRole + 1,
    IdRole,
    ValueRole,
    DescriptionRole,
    SupportRole,
    DateRole,
    MovementRole,
    IsVisibleRole,
    AccountIdRole,
    CategoryRole
  };
  Q_ENUM(Role)

  explicit TransactionListModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role) override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QHash<int, QByteArray> roleNames() const override;

  int count() const { return rowCount(); }
  QList<TransactionPtr> transactions() const { return m_transactions; }
  TransactionPtr transactionAt(int row) const;
  bool contains(const QUuid &transactionId) const;

  void setTransactions(const QList<TransactionPtr> &transactions);
  bool addTransaction(const TransactionPtr &transaction);
  bool removeTransaction(const QUuid &transactionId);
  bool removeTransactionAt(int row);
  void clear();

  Q_INVOKABLE QObject *at(int row) const;
  Q_INVOKABLE QVariantMap get(int row) const;

signals:
  void countChanged();

private:
  int indexOf(const QUuid &transactionId) const;
  int indexOf(const Transaction *transaction) const;
  void emitRolesChanged(Transaction *transaction, const QList<int> &roles);
  void connectTransaction(const TransactionPtr &transaction);
  void disconnectTransaction(Transaction *transaction);
  QVariant roleData(const TransactionPtr &transaction, int role) const;

private:
  QList<TransactionPtr> m_transactions;
};
