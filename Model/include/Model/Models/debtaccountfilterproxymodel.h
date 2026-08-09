#pragma once

#include <QPointer>
#include <QQmlEngine>
#include <QSortFilterProxyModel>
#include <QVariantMap>

#include "account.h"

class MODEL_EXPORT DebtAccountFilterProxyModel : public QSortFilterProxyModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(Account *account READ account WRITE setAccount NOTIFY accountChanged)
  Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
  explicit DebtAccountFilterProxyModel(QObject *parent = nullptr);

  Account *account() const { return m_account; }
  int count() const { return rowCount(); }

  void setAccount(Account *account);

  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE QObject *at(int row) const;
  Q_INVOKABLE QVariantMap get(int row) const;

signals:
  void accountChanged();
  void countChanged();

protected:
  bool filterAcceptsRow(int sourceRow,
                        const QModelIndex &sourceParent) const override;

private:
  void invalidateAccountFilter();

private:
  QPointer<Account> m_account;
};
