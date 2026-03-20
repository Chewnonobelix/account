#include "Model/profile.h"
#include <QMetaType>
#include <QVariant>

/*
 * Implementation of Profile
 * Uses setMetadata()/metaData<T>() from MetaData for storage.
 */

Profile::Profile(QObject *parent) : QObject(parent), MetaData() {
  qRegisterMetaType<QVector<QUuid>>("QVector<QUuid>");
  setId(QUuid());
  setFirstName(QString());
  setLastName(QString());
  setAccounts(QVector<QUuid>());
}

Profile::Profile(const QJsonObject &obj, QObject *parent)
    : QObject(parent), MetaData(obj) {}

// --- Getters ---
QUuid Profile::id() const {
  const QString value = metaData<QString>(KEY_ID);
  return value.isEmpty() ? QUuid() : QUuid(value);
}

QString Profile::firstName() const { return metaData<QString>(KEY_FIRSTNAME); }

QString Profile::lastName() const { return metaData<QString>(KEY_LASTNAME); }

QVector<QUuid> Profile::accounts() const {
  return metaData<QVector<QUuid>>(KEY_ACCOUNTS);
}

// --- Setters ---
void Profile::setId(QUuid id) {
  setMetadata(KEY_ID, id.toString());
  emit profileChanged();
}

void Profile::setFirstName(QString firstName) {
  setMetadata(KEY_FIRSTNAME, firstName);
  emit firstNameChanged();
  emit profileChanged();
}

void Profile::setLastName(QString lastName) {
  setMetadata(KEY_LASTNAME, lastName);
  emit lastNameChanged();
  emit profileChanged();
}

void Profile::setAccounts(QVector<QUuid> accounts) {
  setMetadata(KEY_ACCOUNTS, QVariant::fromValue(accounts));
  emit accountsChanged();
  emit profileChanged();
}

// --- Serialization ---
QJsonObject Profile::toJson() const {
  QJsonObject obj;
  obj.insert(KEY_ID, id().toString());
  obj.insert(KEY_FIRSTNAME, firstName());
  obj.insert(KEY_LASTNAME, lastName());

  QJsonArray arr;
  for (const QUuid &u : accounts())
    arr.append(u.toString());
  obj.insert(KEY_ACCOUNTS, arr);
  return obj;
}

// --- Validation ---
bool Profile::isValid() const {
  return !id().isNull() && !lastName().isEmpty();
}

// --- Helpers ---
QJsonArray Profile::accountsToJsonArray(const QVector<QUuid> &accounts) {
  QJsonArray arr;
  for (const QUuid &u : accounts)
    arr.append(u.toString());
  return arr;
}

QVector<QUuid> Profile::jsonArrayToAccounts(const QJsonArray &array) {
  QVector<QUuid> vec;
  vec.reserve(array.size());
  for (const QJsonValue &v : array)
    if (v.isString())
      vec.append(QUuid(v.toString()));
  return vec;
}
