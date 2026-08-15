#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QQmlEngine>
#include <QVariantMap>

#include "category.h"
#include "model_global.h"

class MODEL_EXPORT CategoryListModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
  enum Role {
    CategoryRole = Qt::UserRole + 1,
    IdRole,
    NameRole,
    DirectionRole
  };
  Q_ENUM(Role)

  explicit CategoryListModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role) override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QHash<int, QByteArray> roleNames() const override;

  int count() const { return rowCount(); }
  QList<CategoryPtr> categories() const { return m_categories; }
  CategoryPtr categoryAt(int row) const;
  bool contains(const QUuid &categoryId) const;

  void setCategories(const QList<CategoryPtr> &categories);
  bool addCategory(const CategoryPtr &category);
  // QML can't construct a CategoryPtr (QSharedPointer) directly, so this
  // overload wraps a QObject-owned Category (e.g. from Component.createObject)
  // with a no-op deleter: the shared pointer is only used for shared access
  // here, actual lifetime stays with whatever already owns the QObject.
  Q_INVOKABLE bool addCategory(QObject *category);
  bool removeCategory(const QUuid &categoryId);
  bool removeCategoryAt(int row);
  Q_INVOKABLE void clear();

  Q_INVOKABLE QObject *at(int row) const;
  Q_INVOKABLE QVariantMap get(int row) const;
  Q_INVOKABLE int indexOf(const QUuid &categoryId) const;

signals:
  void countChanged();

private:
  int indexOf(const Category *category) const;
  void emitRolesChanged(Category *category, const QList<int> &roles);
  void connectCategory(const CategoryPtr &category);
  void disconnectCategory(Category *category);
  QVariant roleData(const CategoryPtr &category, int role) const;

private:
  QList<CategoryPtr> m_categories;
};
