#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QQmlEngine>
#include <QVariantMap>

#include "debt.h"
#include "model_global.h"

class MODEL_EXPORT DebtListModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
  enum Role {
    DebtRole = Qt::UserRole + 1,
    IdRole,
    NameRole,
    CounterpartyRole,
    PrincipalRole,
    RemainingRole,
    InterestRole,
    StartDateRole,
    EndDateRole,
    RecurrenceRole,
    DirectionRole,
    DescriptionRole,
    AccountIdRole
  };
  Q_ENUM(Role)

  explicit DebtListModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role) override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QHash<int, QByteArray> roleNames() const override;

  int count() const { return rowCount(); }
  QList<DebtPtr> debts() const { return m_debts; }
  DebtPtr debtAt(int row) const;
  bool contains(const QUuid &debtId) const;

  void setDebts(const QList<DebtPtr> &debts);
  bool addDebt(const DebtPtr &debt);
  // QML can't construct a DebtPtr (QSharedPointer) directly, so this
  // overload wraps a QObject-owned Debt (e.g. from Component.createObject)
  // with a no-op deleter: the shared pointer is only used for shared access
  // here, actual lifetime stays with whatever already owns the QObject.
  Q_INVOKABLE bool addDebt(QObject *debt);
  bool removeDebt(const QUuid &debtId);
  bool removeDebtAt(int row);
  void clear();

  Q_INVOKABLE QObject *at(int row) const;
  Q_INVOKABLE QVariantMap get(int row) const;
  Q_INVOKABLE int indexOf(const QUuid &debtId) const;

signals:
  void countChanged();

private:
  int indexOf(const Debt *debt) const;
  void emitRolesChanged(Debt *debt, const QList<int> &roles);
  void connectDebt(const DebtPtr &debt);
  void disconnectDebt(Debt *debt);
  QVariant roleData(const DebtPtr &debt, int role) const;

private:
  QList<DebtPtr> m_debts;
};
