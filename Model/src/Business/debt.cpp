#include "Model/Business/debt.h"
#include "Model/Business/frequency.h"

#include <QJsonValue>

Debt::Debt(QObject* parent) : QObject(parent), MetaData() {
    setMetadata(Key::id, QUuid::createUuid());
    setMetadata(Key::name, QString());
    setMetadata(Key::counterparty, QString());
    setMetadata(Key::principal, 0.0);
    setMetadata(Key::remaining, 0.0);
    setMetadata(Key::interest, 0.0);
    setMetadata(Key::startDate, QDate());
    setMetadata(Key::endDate, QDate());
    setMetadata(Key::recurrence, OpenAccountEnums::Frequency::Once);
    setMetadata(Key::direction, OpenAccountEnums::Movement::Debit);
    setMetadata(Key::description, QString());
    setMetadata(Key::accountId, QUuid());
}

Debt::Debt(const QJsonObject& json, QObject* parent)
    : QObject(parent), MetaData() {
    setMetadata(Key::id, QUuid::createUuid());
    setMetadata(Key::name, QString());
    setMetadata(Key::counterparty, QString());
    setMetadata(Key::principal, 0.0);
    setMetadata(Key::remaining, 0.0);
    setMetadata(Key::interest, 0.0);
    setMetadata(Key::startDate, QDate());
    setMetadata(Key::endDate, QDate());
    setMetadata(Key::recurrence, OpenAccountEnums::Frequency::Once);
    setMetadata(Key::direction, OpenAccountEnums::Movement::Debit);
    setMetadata(Key::description, QString());
    setMetadata(Key::accountId, QUuid());
    Debt::fromJson(json);
}

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

// --- Setters ---------------------------------------------------------------

void Debt::setId(QUuid value) {
    if (id() == value)
        return;

    setMetadata(Key::id, value);
    emit idChanged();
    emit changed();
}

void Debt::setName(QString value) {
    if (name() == value)
        return;

    setMetadata(Key::name, value);
    emit nameChanged();
    emit changed();
}

void Debt::setCounterparty(QString value) {
    if (counterparty() == value)
        return;

    setMetadata(Key::counterparty, value);
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
        setMetadata(Key::principal, value);
        emit principalChanged();
    }

    if (remainingWasChanged) {
        setMetadata(Key::remaining, nextRemaining);
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

    setMetadata(Key::remaining, value);
    emit remainingChanged();
    emit changed();
}

void Debt::setInterest(double value) {
    if (value < 0.0)
        value = 0.0;

    if (qFuzzyCompare(interest() + 1.0, value + 1.0))
        return;

    setMetadata(Key::interest, value);
    emit interestChanged();
    emit changed();
}

void Debt::setStartDate(QDate value) {
    if (startDate() == value)
        return;

    setMetadata(Key::startDate, value);
    emit startDateChanged();
    emit changed();
}

void Debt::setEndDate(QDate value) {
    if (endDate() == value)
        return;

    setMetadata(Key::endDate, value);
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

    setMetadata(Key::recurrence, value);
    emit recurrenceChanged();
    emit changed();
}

void Debt::setDirection(OpenAccountEnums::Movement value) {
    if (value == OpenAccountEnums::Movement::Both)
        value = OpenAccountEnums::Movement::Debit;

    if (direction() == value)
        return;

    setMetadata(Key::direction, value);
    emit directionChanged();
    emit changed();
}

void Debt::setDescription(QString value) {
    if (description() == value)
        return;

    setMetadata(Key::description, value);
    emit descriptionChanged();
    emit changed();
}

void Debt::setAccountId(QUuid value) {
    if (accountId() == value)
        return;

    setMetadata(Key::accountId, value);
    emit accountIdChanged();
    emit changed();
}

// --- JSON ------------------------------------------------------------------

QJsonObject Debt::toJson() const {
    // Qualified call to the base serializer: an unqualified toJson() (or a
    // static_cast<QJsonObject> going through MetaData::operator QJsonObject)
    // would re-dispatch to this override and recurse infinitely.
    QJsonObject json = MetaData::toJson();
    json.insert(Key::recurrence, enumToJson(recurrence()));
    json.insert(Key::direction, enumToJson(direction()));
    return json;
}

void Debt::fromJson(const QJsonObject& json) {
    // MetaData::fromJson() restores id/name/counterparty/principal/
    // remaining/interest/startDate/endDate/description/accountId;
    // recurrence/direction are patched afterwards, with the same
    // normalization guards setRecurrence()/setDirection() apply, but via
    // setMetadata() directly so restoring from JSON stays signal-free like
    // every other constructor path in this class.
    MetaData::fromJson(json);

    OpenAccountEnums::Frequency parsedRecurrence =
        enumFromJson<OpenAccountEnums::Frequency>(
            json.value(Key::recurrence), OpenAccountEnums::Frequency::Once);
    if (parsedRecurrence == OpenAccountEnums::Frequency::Custom ||
        parsedRecurrence == OpenAccountEnums::Frequency::LAST) {
        parsedRecurrence = OpenAccountEnums::Frequency::Once;
    }
    setMetadata(Key::recurrence, parsedRecurrence);

    OpenAccountEnums::Movement parsedDirection =
        enumFromJson<OpenAccountEnums::Movement>(
            json.value(Key::direction), OpenAccountEnums::Movement::Debit);
    if (parsedDirection == OpenAccountEnums::Movement::Both)
        parsedDirection = OpenAccountEnums::Movement::Debit;
    setMetadata(Key::direction, parsedDirection);
}
