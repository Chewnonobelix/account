#include "Model/account.h"

Account::Account(QObject *parent) : QObject(parent), MetaData() {
  setNumber(QString());
  setBank(QString());
  setInterest(0.0);
  setDescription(QString());
  setOpening(QDate());
  setId(QUuid());
}

Account::Account(const QJsonObject &json, QObject *parent)
    : QObject(parent), MetaData(json) {}

Account::Account(const Account &other)
    : QObject(other.parent()), MetaData(other) {}

Account &Account::operator=(const Account &other) {
  if (this == &other)
    return *this;
  MetaData::operator=(other);
  emit changed();
  return *this;
}

Account::Account(Account &&other) noexcept
    : QObject(other.parent()), MetaData(std::move(other)) {}

Account &Account::operator=(Account &&other) noexcept {
  if (this == &other)
    return *this;
  MetaData::operator=(std::move(other));
  emit changed();
  return *this;
}

QJsonObject Account::toJson() const {
  QJsonObject o;
  o.insert(Key::number, number());
  o.insert(Key::bank, bank());
  o.insert(Key::interest, interest());
  o.insert(Key::description, description());
  if (opening().isValid())
    o.insert(Key::opening, opening().toString(Qt::ISODate));
  else
    o.insert(Key::opening, QJsonValue());
  if (!id().isNull())
    o.insert(Key::id, id().toString(QUuid::WithoutBraces));
  else
    o.insert(Key::id, QJsonValue());
  return o;
}
