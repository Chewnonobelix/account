#include "Model/debt.h"
#include "Model/frequency.h"

#include <QJsonValue>
#include <QMetaEnum>

Debt::Debt(QObject *parent) : QObject(parent), MetaData() {
  setMetadata(Key::Id, QUuid::createUuid());
  setMetadata(Key::Name, QString());
  setMetadata(Key::Counterparty, QString());
  setMetadata(Key::Principal, 0.0);
  setMetadata(Key::Remaining, 0.0);
  setMetadata(Key::Interest, 0.0);
  setMetadata(Key::StartDate, QDate());
  setMetadata(Key::EndDate, QDate());
  setMetadata(Key::Recurrence, OpenAccountEnums::Frequency::Once);
  setMetadata(Key::Direction, OpenAccountEnums::Movement::Debit);
  setMetadata(Key::Description, QString());
  setMetadata(Key::AccountId, QUuid());
}

Debt::Debt(const QJsonObject &json, QObject *parent)
    : QObject(parent), MetaData(json) {
  OpenAccountEnums::Frequency parsedRecurrence = OpenAccountEnums::Frequency::Once;
  if (json.value(Key::Recurrence).isString()) {
    const QMetaEnum metaEnum =
        QMetaEnum::fromType<OpenAccountEnums::Frequency>();
    const QByteArray utf8 = json.value(Key::Recurrence).toString().toUtf8();
    const int enumValue = metaEnum.keyToValue(utf8.constData());
    if (enumValue >= 0)
      parsedRecurrence = static_cast<OpenAccountEnums::Frequency>(enumValue);
  } else if (json.value(Key::Recurrence).isDouble()) {
    parsedRecurrence = static_cast<OpenAccountEnums::Frequency>(
        json.value(Key::Recurrence).toInt());
  }

  if (parsedRecurrence == OpenAccountEnums::Frequency::Custom ||
      parsedRecurrence == OpenAccountEnums::Frequency::LAST) {
    parsedRecurrence = OpenAccountEnums::Frequency::Once;
  }

  OpenAccountEnums::Movement parsedDirection = OpenAccountEnums::Movement::Debit;
  if (json.value(Key::Direction).isString()) {
    const QMetaEnum metaEnum =
        QMetaEnum::fromType<OpenAccountEnums::Movement>();
    const QByteArray utf8 = json.value(Key::Direction).toString().toUtf8();
    const int enumValue = metaEnum.keyToValue(utf8.constData());
    if (enumValue >= 0)
      parsedDirection = static_cast<OpenAccountEnums::Movement>(enumValue);
  } else if (json.value(Key::Direction).isDouble()) {
    parsedDirection = static_cast<OpenAccountEnums::Movement>(
        json.value(Key::Direction).toInt());
  }

  if (parsedDirection == OpenAccountEnums::Movement::Both)
    parsedDirection = OpenAccountEnums::Movement::Debit;

  setMetadata(Key::Recurrence, parsedRecurrence);
  setMetadata(Key::Direction, parsedDirection);
}

Debt::~Debt() = default;

QUuid Debt::id() const { return metaData<QUuid>(Key::Id); }

QString Debt::name() const { return metaData<QString>(Key::Name); }

QString Debt::counterparty() const { return metaData<QString>(Key::Counterparty); }

double Debt::principal() const { return metaData<double>(Key::Principal); }

double Debt::remaining() const { return metaData<double>(Key::Remaining); }

double Debt::interest() const { return metaData<double>(Key::Interest); }

QDate Debt::startDate() const { return metaData<QDate>(Key::StartDate); }

QDate Debt::endDate() const { return metaData<QDate>(Key::EndDate); }

OpenAccountEnums::Frequency Debt::recurrence() const {
  return metaData<OpenAccountEnums::Frequency>(Key::Recurrence);
}

OpenAccountEnums::Movement Debt::direction() const {
  return metaData<OpenAccountEnums::Movement>(Key::Direction);
}

QString Debt::description() const { return metaData<QString>(Key::Description); }

QUuid Debt::accountId() const { return metaData<QUuid>(Key::AccountId); }

bool Debt::isClosed() const { return remaining() <= 0.0; }

QList<TransactionPtr> Debt::generateRepayments() const {
  QList<TransactionPtr> repayments;

  if (!startDate().isValid() || !endDate().isValid() || startDate() > endDate())
    return repayments;

  if (principal() <= 0.0)
    return repayments;

  TransactionPtr prototype = TransactionPtr::create();
  prototype->setDescription(description().trimmed().isEmpty() ? name()
                                                              : description());
  prototype->setSupport(OpenAccountEnums::Support::Transfer);
  prototype->setDate(startDate());
  prototype->setMovement(direction());
  prototype->setIsVisible(true);
  prototype->setAccountId(accountId());

  Frequency frequency;
  frequency.setId(id());
  frequency.setFrequency(recurrence());
  frequency.setPrototype(prototype);

  repayments = frequency.generate(startDate(), endDate());

  if (repayments.isEmpty())
    return repayments;

  const double totalAmount = principal() + (principal() * interest() / 100.0);
  const qint64 totalCents = qRound64(totalAmount * 100.0);
  const qint64 baseCents = totalCents / repayments.size();
  const qint64 remainder = totalCents % repayments.size();
  qint64 repaymentTotalCents = 0;

  for (int i = 0; i < repayments.size(); ++i) {
    const qint64 cents = baseCents + (i < remainder ? 1 : 0);
    repayments[i]->setValue(static_cast<double>(cents) / 100.0);
    repayments[i]->setMetadata(QStringLiteral("debtId"), id());
    repaymentTotalCents += qRound64(repayments[i]->value() * 100.0);
  }

  Q_ASSERT_X(repaymentTotalCents == totalCents, "Debt::generateRepayments",
             "The sum of repayments must match principal plus interest.");

  return repayments;
}

void Debt::setId(QUuid value) {
  if (id() == value)
    return;

  setMetadata(Key::Id, value);
  emit idChanged();
  emit changed();
}

void Debt::setName(QString value) {
  if (name() == value)
    return;

  setMetadata(Key::Name, value);
  emit nameChanged();
  emit changed();
}

void Debt::setCounterparty(QString value) {
  if (counterparty() == value)
    return;

  setMetadata(Key::Counterparty, value);
  emit counterpartyChanged();
  emit changed();
}

void Debt::setPrincipal(double value) {
  if (value < 0.0)
    value = 0.0;

  const double currentPrincipal = principal();
  const double currentRemaining = remaining();
  double nextRemaining = qMin(currentRemaining, value);

  if (qFuzzyCompare(currentPrincipal + 1.0, 1.0) &&
      qFuzzyCompare(currentRemaining + 1.0, 1.0) && value > 0.0) {
    nextRemaining = value;
  }

  const bool principalWasChanged =
      !qFuzzyCompare(currentPrincipal + 1.0, value + 1.0);
  const bool remainingWasChanged =
      !qFuzzyCompare(currentRemaining + 1.0, nextRemaining + 1.0);

  if (!principalWasChanged && !remainingWasChanged)
    return;

  if (principalWasChanged) {
    setMetadata(Key::Principal, value);
    emit principalChanged();
  }

  if (remainingWasChanged) {
    setMetadata(Key::Remaining, nextRemaining);
    emit remainingChanged();
  }

  emit changed();
}

void Debt::setRemaining(double value) {
  if (value < 0.0)
    value = 0.0;
  value = qMin(value, principal());

  if (qFuzzyCompare(remaining() + 1.0, value + 1.0))
    return;

  setMetadata(Key::Remaining, value);
  emit remainingChanged();
  emit changed();
}

void Debt::setInterest(double value) {
  if (value < 0.0)
    value = 0.0;

  if (qFuzzyCompare(interest() + 1.0, value + 1.0))
    return;

  setMetadata(Key::Interest, value);
  emit interestChanged();
  emit changed();
}

void Debt::setStartDate(QDate value) {
  if (startDate() == value)
    return;

  setMetadata(Key::StartDate, value);
  emit startDateChanged();
  emit changed();
}

void Debt::setEndDate(QDate value) {
  if (endDate() == value)
    return;

  setMetadata(Key::EndDate, value);
  emit endDateChanged();
  emit changed();
}

void Debt::setRecurrence(OpenAccountEnums::Frequency value) {
  if (value == OpenAccountEnums::Frequency::Custom ||
      value == OpenAccountEnums::Frequency::LAST) {
    value = OpenAccountEnums::Frequency::Once;
  }

  if (recurrence() == value)
    return;

  setMetadata(Key::Recurrence, value);
  emit recurrenceChanged();
  emit changed();
}

void Debt::setDirection(OpenAccountEnums::Movement value) {
  if (value == OpenAccountEnums::Movement::Both)
    value = OpenAccountEnums::Movement::Debit;

  if (direction() == value)
    return;

  setMetadata(Key::Direction, value);
  emit directionChanged();
  emit changed();
}

void Debt::setDescription(QString value) {
  if (description() == value)
    return;

  setMetadata(Key::Description, value);
  emit descriptionChanged();
  emit changed();
}

void Debt::setAccountId(QUuid value) {
  if (accountId() == value)
    return;

  setMetadata(Key::AccountId, value);
  emit accountIdChanged();
  emit changed();
}

QJsonObject Debt::toJson() const {
  QJsonObject json = static_cast<QJsonObject>(*this);

  const QMetaEnum recurrenceMetaEnum =
      QMetaEnum::fromType<OpenAccountEnums::Frequency>();
  const char *recurrenceKey =
      recurrenceMetaEnum.valueToKey(static_cast<int>(recurrence()));
  json.insert(Key::Recurrence,
              recurrenceKey != nullptr ? QString::fromLatin1(recurrenceKey)
                                       : QStringLiteral("Once"));

  const QMetaEnum metaEnum = QMetaEnum::fromType<OpenAccountEnums::Movement>();
  const char *directionKey =
      metaEnum.valueToKey(static_cast<int>(direction()));
  json.insert(Key::Direction,
              directionKey != nullptr ? QString::fromLatin1(directionKey)
                                      : QStringLiteral("Debit"));

  return json;
}
