#include "Model/frequency.h"

#include <QJsonValue>

Frequency::Frequency(QObject *parent) : QObject(parent) {
  setMetadata(Key::Id, QUuid::createUuid());
  setMetadata(Key::Frequency,
              QVariant::fromValue(OpenAccountEnums::Frequency::Once));
  setMetadata(Key::DateFormat, QStringLiteral("yyyy-MM-dd"));
  setMetadata(Key::CustomIntervalDays, 1);
  setMetadata(Key::Prototype, QVariant::fromValue(TransactionPtr()));
}

Frequency::Frequency(const QJsonObject &json, QObject *parent)
    : MetaData(json), QObject(parent) {}

QUuid Frequency::id() const { return metaData<QUuid>(Key::Id); }

OpenAccountEnums::Frequency Frequency::frequency() const {
  return metaData<OpenAccountEnums::Frequency>(Key::Frequency);
}

QString Frequency::dateFormat() const {
  return metaData<QString>(Key::DateFormat);
}

int Frequency::customIntervalDays() const {
  return metaData<int>(Key::CustomIntervalDays);
}

TransactionPtr Frequency::prototype() const {
  return metaData<TransactionPtr>(Key::Prototype);
}

void Frequency::setId(QUuid value) {
  assignIfChanged(Key::Id, value, &Frequency::idChanged);
}

void Frequency::setFrequency(OpenAccountEnums::Frequency value) {
  assignIfChanged(Key::Frequency, value, &Frequency::frequencyChanged);
}

void Frequency::setDateFormat(QString value) {
  if (value.isEmpty())
    value = QStringLiteral("yyyy-MM-dd");

  assignIfChanged(Key::DateFormat, value, &Frequency::dateFormatChanged);
}

void Frequency::setCustomIntervalDays(int value) {
  if (value < 1)
    value = 1;

  assignIfChanged(Key::CustomIntervalDays, value,
                  &Frequency::customIntervalDaysChanged);
}

void Frequency::setPrototype(TransactionPtr value) {
  assignIfChanged(Key::Prototype, value, &Frequency::prototypeChanged);
}

QJsonObject Frequency::toJson() const {
  QJsonObject json;
  json.insert(Key::Id, id().toString(QUuid::WithoutBraces));
  json.insert(Key::Frequency, static_cast<int>(frequency()));
  json.insert(Key::DateFormat, dateFormat());
  json.insert(Key::CustomIntervalDays, customIntervalDays());
  json.insert(Key::Prototype,
              prototype() ? prototype()->toJson() : QJsonObject());
  return json;
}

QList<TransactionPtr> Frequency::generate(QDate from, QDate to) const {
  QList<TransactionPtr> result;
  const TransactionPtr proto = prototype();

  if (!proto || !from.isValid() || !to.isValid() || from > to)
    return result;

  QDate current = from;

  while (current.isValid() && current <= to) {
    result.append(createTransaction(proto, current));

    const QDate next = nextDate(current);
    if (!next.isValid() || next <= current)
      break;

    current = next;
  }

  return result;
}

QList<TransactionPtr> Frequency::generate(QDate startDate, int count) const {
  QList<TransactionPtr> result;
  const TransactionPtr proto = prototype();

  if (!proto || !startDate.isValid() || count <= 0)
    return result;

  QDate current = startDate;

  for (int i = 0; i < count; ++i) {
    result.append(createTransaction(proto, current));

    const QDate next = nextDate(current);
    if (!next.isValid() || next <= current)
      break;

    current = next;
  }

  return result;
}

TransactionPtr Frequency::createTransaction(TransactionPtr proto,
                                            QDate date) const {
  TransactionPtr transaction = TransactionPtr::create(proto->toJson());

  // generate new unique id
  transaction->setId(QUuid::createUuid());
  transaction->setDate(date);
  transaction->setDescription(buildDescription(proto->description(), date));
  transaction->setMetadata(TransactionKey::FrequencyId, id());
  return transaction;
}

QString Frequency::buildDescription(QString baseDescription, QDate date) const {
  const QString formattedDate = date.toString(dateFormat());

  if (baseDescription.trimmed().isEmpty())
    return formattedDate;

  return QStringLiteral("%1 - %2").arg(baseDescription, formattedDate);
}

QDate Frequency::nextDate(QDate current) const {
  if (frequency() == OpenAccountEnums::Frequency::Custom)
    return current.addDays(customIntervalDays());

  return current.addDays(OpenAccountEnums::dayToNext(current, frequency()));
}
