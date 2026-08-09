#pragma once

#include <QPointer>
#include <QQmlEngine>
#include <QSortFilterProxyModel>
#include <QVariantMap>

#include "profile.h"

class MODEL_EXPORT ProfileAccountFilterProxyModel
    : public QSortFilterProxyModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(Profile *profile READ profile WRITE setProfile NOTIFY profileChanged)
  Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
  explicit ProfileAccountFilterProxyModel(QObject *parent = nullptr);

  Profile *profile() const { return m_profile; }
  int count() const { return rowCount(); }

  void setProfile(Profile *profile);

  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE QObject *at(int row) const;
  Q_INVOKABLE QVariantMap get(int row) const;

signals:
  void profileChanged();
  void countChanged();

protected:
  bool filterAcceptsRow(int sourceRow,
                        const QModelIndex &sourceParent) const override;

private:
  void invalidateProfileFilter();

private:
  QPointer<Profile> m_profile;
};
