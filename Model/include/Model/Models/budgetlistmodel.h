#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QQmlEngine>
#include <QVariantMap>

#include "budget.h"
#include "model_global.h"

class MODEL_EXPORT BudgetListModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
  enum Role {
    BudgetRole = Qt::UserRole + 1,
    IdRole,
    NameRole,
    CategoryIdRole,
    ThresholdRole,
    StartDateRole,
    EndDateRole,
    RecurrenceRole
  };
  Q_ENUM(Role)

  explicit BudgetListModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role) override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QHash<int, QByteArray> roleNames() const override;

  int count() const { return rowCount(); }
  QList<BudgetPtr> budgets() const { return m_budgets; }
  BudgetPtr budgetAt(int row) const;
  bool contains(const QUuid &budgetId) const;

  void setBudgets(const QList<BudgetPtr> &budgets);
  bool addBudget(const BudgetPtr &budget);
  // QML can't construct a BudgetPtr (QSharedPointer) directly, so this
  // overload wraps a QObject-owned Budget (e.g. from Component.createObject)
  // with a no-op deleter: the shared pointer is only used for shared access
  // here, actual lifetime stays with whatever already owns the QObject.
  Q_INVOKABLE bool addBudget(QObject *budget);
  bool removeBudget(const QUuid &budgetId);
  bool removeBudgetAt(int row);
  Q_INVOKABLE void clear();

  Q_INVOKABLE QObject *at(int row) const;
  Q_INVOKABLE QVariantMap get(int row) const;
  Q_INVOKABLE int indexOf(const QUuid &budgetId) const;

signals:
  void countChanged();

private:
  int indexOf(const Budget *budget) const;
  void emitRolesChanged(Budget *budget, const QList<int> &roles);
  void connectBudget(const BudgetPtr &budget);
  void disconnectBudget(Budget *budget);
  QVariant roleData(const BudgetPtr &budget, int role) const;

private:
  QList<BudgetPtr> m_budgets;
};
