#include "Model/Business/budget.h"

Budget::Budget(QObject* parent) : QObject(parent), MetaData() {
    setMetadata(Key::id, QUuid::createUuid());
    setMetadata(Key::name, QString());
    setMetadata(Key::categoryId, QUuid());
    setMetadata(Key::threshold, 0.0);
    setMetadata(Key::startDate, QDate());
    setMetadata(Key::endDate, QDate());
    setMetadata(Key::recurrence, OpenAccountEnums::Frequency::Once);
}

Budget::Budget(const QJsonObject& json, QObject* parent) : QObject(parent), MetaData() {
    setMetadata(Key::id, QUuid::createUuid());
    setMetadata(Key::name, QString());
    setMetadata(Key::categoryId, QUuid());
    setMetadata(Key::threshold, 0.0);
    setMetadata(Key::startDate, QDate());
    setMetadata(Key::endDate, QDate());
    setMetadata(Key::recurrence, OpenAccountEnums::Frequency::Once);
    Budget::fromJson(json);
}

bool Budget::isWithinPeriod(QDate date) const {
    if (!startDate().isValid() || !endDate().isValid())
        return false;

    return date >= startDate() && date <= endDate();
}

bool Budget::matches(const TransactionPtr& transaction) const {
    if (!transaction)
        return false;

    return transaction->category() == categoryId() && isWithinPeriod(transaction->date());
}

bool Budget::addTransaction(const TransactionPtr& transaction) {
    if (!matches(transaction))
        return false;

    for (const TransactionPtr& existing : std::as_const(m_transactions))
        if (existing->id() == transaction->id())
            return false;

    m_transactions.append(transaction);
    emit usageChanged();
    emit changed();
    return true;
}

void Budget::removeTransaction(QUuid transactionId) {
    const qsizetype removed = m_transactions.removeIf(
        [transactionId](const TransactionPtr& t) { return t->id() == transactionId; });

    if (removed == 0)
        return;

    emit usageChanged();
    emit changed();
}

void Budget::clearTransactions() {
    if (m_transactions.isEmpty())
        return;

    m_transactions.clear();
    emit usageChanged();
    emit changed();
}

double Budget::usedAmount() const {
    double total = 0.0;
    for (const TransactionPtr& t : std::as_const(m_transactions))
        total += qAbs(t->value());
    return total;
}

double Budget::remaining() const { return threshold() - usedAmount(); }

double Budget::usageRatio() const {
    if (threshold() <= 0.0)
        return 0.0;
    return (usedAmount() / threshold()) * 100.0;
}

bool Budget::isOverBudget() const { return threshold() > 0.0 && usedAmount() > threshold(); }

// --- Setters ---------------------------------------------------------------

void Budget::setId(QUuid value) {
    if (id() == value)
        return;

    setMetadata(Key::id, value);
    emit idChanged();
    emit changed();
}

void Budget::setName(QString value) {
    if (name() == value)
        return;

    setMetadata(Key::name, value);
    emit nameChanged();
    emit changed();
}

void Budget::setCategoryId(QUuid value) {
    if (categoryId() == value)
        return;

    setMetadata(Key::categoryId, value);
    emit categoryIdChanged();
    emit changed();
}

void Budget::setThreshold(double value) {
    if (value < 0.0)
        value = 0.0;

    if (qFuzzyCompare(threshold() + 1.0, value + 1.0))
        return;

    setMetadata(Key::threshold, value);
    emit thresholdChanged();
    emit usageChanged();
    emit changed();
}

void Budget::setStartDate(QDate value) {
    if (startDate() == value)
        return;

    setMetadata(Key::startDate, value);
    emit startDateChanged();
    emit changed();
}

void Budget::setEndDate(QDate value) {
    if (endDate() == value)
        return;

    setMetadata(Key::endDate, value);
    emit endDateChanged();
    emit changed();
}

void Budget::setRecurrence(OpenAccountEnums::Frequency value) {
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

// --- JSON --------------------------------------------------------------

QJsonObject Budget::toJson() const {
    // Qualified call to the base serializer: an unqualified toJson() would
    // re-dispatch to this override and recurse infinitely. MetaData already
    // knows how to serialize id/categoryId (QUuid), name (QString),
    // threshold (double) and startDate/endDate (QDate); only the enum needs
    // custom handling.
    QJsonObject json = MetaData::toJson();
    json.insert(Key::recurrence, enumToJson(recurrence()));
    return json;
}

void Budget::fromJson(const QJsonObject& json) {
    // MetaData::fromJson() restores id/categoryId/name/threshold/startDate/
    // endDate; recurrence is patched afterwards via setRecurrence() so that
    // its normalization guard and signal both apply.
    MetaData::fromJson(json);
    setRecurrence(enumFromJson<OpenAccountEnums::Frequency>(
        json.value(Key::recurrence), OpenAccountEnums::Frequency::Once));
}
