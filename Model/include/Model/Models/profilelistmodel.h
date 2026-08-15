#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QQmlEngine>
#include <QVariantMap>

#include "model_global.h"
#include "profile.h"

class MODEL_EXPORT ProfileListModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
  enum Role {
    ProfileRole = Qt::UserRole + 1,
    IdRole,
    FirstNameRole,
    LastNameRole,
    AccountsRole
  };
  Q_ENUM(Role)

  explicit ProfileListModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role) override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QHash<int, QByteArray> roleNames() const override;

  int count() const { return rowCount(); }
  QList<ProfilePtr> profiles() const { return m_profiles; }
  ProfilePtr profileAt(int row) const;
  bool contains(const QUuid &profileId) const;

  void setProfiles(const QList<ProfilePtr> &profiles);
  bool addProfile(const ProfilePtr &profile);
  // QML can't construct a ProfilePtr (QSharedPointer) directly, so this
  // overload wraps a QObject-owned Profile (e.g. from Component.createObject)
  // with a no-op deleter: the shared pointer is only used for shared access
  // here, actual lifetime stays with whatever already owns the QObject.
  Q_INVOKABLE bool addProfile(QObject *profile);
  bool removeProfile(const QUuid &profileId);
  bool removeProfileAt(int row);
  Q_INVOKABLE void clear();

  Q_INVOKABLE QObject *at(int row) const;
  Q_INVOKABLE QVariantMap get(int row) const;
  Q_INVOKABLE int indexOf(const QUuid &profileId) const;

signals:
  void countChanged();

private:
  int indexOf(const Profile *profile) const;
  void emitRolesChanged(Profile *profile, const QList<int> &roles);
  void connectProfile(const ProfilePtr &profile);
  void disconnectProfile(Profile *profile);
  QVariant roleData(const ProfilePtr &profile, int role) const;

private:
  QList<ProfilePtr> m_profiles;
};
