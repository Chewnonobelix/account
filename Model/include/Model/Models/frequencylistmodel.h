#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QQmlEngine>
#include <QVariantMap>

#include "frequency.h"
#include "model_global.h"

class MODEL_EXPORT FrequencyListModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
  enum Role {
    FrequencyRole = Qt::UserRole + 1,
    IdRole,
    IntervalRole,
    DateFormatRole,
    CustomIntervalDaysRole,
    PrototypeRole
  };
  Q_ENUM(Role)

  explicit FrequencyListModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role) override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QHash<int, QByteArray> roleNames() const override;

  int count() const { return rowCount(); }
  QList<FrequencyPtr> frequencies() const { return m_frequencies; }
  FrequencyPtr frequencyAt(int row) const;
  bool contains(const QUuid &frequencyId) const;

  void setFrequencies(const QList<FrequencyPtr> &frequencies);
  bool addFrequency(const FrequencyPtr &frequency);
  // QML can't construct a FrequencyPtr (QSharedPointer) directly, so this
  // overload wraps a QObject-owned Frequency (e.g. from
  // Component.createObject) with a no-op deleter: the shared pointer is only
  // used for shared access here, actual lifetime stays with whatever already
  // owns the QObject.
  Q_INVOKABLE bool addFrequency(QObject *frequency);
  bool removeFrequency(const QUuid &frequencyId);
  bool removeFrequencyAt(int row);
  void clear();

  Q_INVOKABLE QObject *at(int row) const;
  Q_INVOKABLE QVariantMap get(int row) const;
  Q_INVOKABLE int indexOf(const QUuid &frequencyId) const;

signals:
  void countChanged();

private:
  int indexOf(const Frequency *frequency) const;
  void emitRolesChanged(Frequency *frequency, const QList<int> &roles);
  void connectFrequency(const FrequencyPtr &frequency);
  void disconnectFrequency(Frequency *frequency);
  QVariant roleData(const FrequencyPtr &frequency, int role) const;

private:
  QList<FrequencyPtr> m_frequencies;
};
