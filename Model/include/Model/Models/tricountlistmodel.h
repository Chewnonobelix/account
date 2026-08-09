#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QQmlEngine>
#include <QVariantMap>

#include "model_global.h"
#include "tricount.h"

class MODEL_EXPORT TricountListModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
  enum Role {
    TricountRole = Qt::UserRole + 1,
    IdRole,
    NameRole,
    DescriptionRole,
    DateRole,
    CurrencyRole,
    MembersRole,
    ExpensesRole
  };
  Q_ENUM(Role)

  explicit TricountListModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role) override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QHash<int, QByteArray> roleNames() const override;

  int count() const { return rowCount(); }
  QList<TricountPtr> tricounts() const { return m_tricounts; }
  TricountPtr tricountAt(int row) const;
  bool contains(const QUuid &tricountId) const;

  void setTricounts(const QList<TricountPtr> &tricounts);
  bool addTricount(const TricountPtr &tricount);
  // QML can't construct a TricountPtr (QSharedPointer) directly, so this
  // overload wraps a QObject-owned Tricount (e.g. from
  // Component.createObject) with a no-op deleter: the shared pointer is only
  // used for shared access here, actual lifetime stays with whatever already
  // owns the QObject.
  Q_INVOKABLE bool addTricount(QObject *tricount);
  bool removeTricount(const QUuid &tricountId);
  bool removeTricountAt(int row);
  void clear();

  Q_INVOKABLE QObject *at(int row) const;
  Q_INVOKABLE QVariantMap get(int row) const;
  Q_INVOKABLE int indexOf(const QUuid &tricountId) const;

signals:
  void countChanged();

private:
  int indexOf(const Tricount *tricount) const;
  void emitRolesChanged(Tricount *tricount, const QList<int> &roles);
  void connectTricount(const TricountPtr &tricount);
  void disconnectTricount(Tricount *tricount);
  QVariant roleData(const TricountPtr &tricount, int role) const;

private:
  QList<TricountPtr> m_tricounts;
};
