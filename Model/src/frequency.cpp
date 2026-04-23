#include "Model/frequency.h"

namespace {
constexpr auto kDefaultDateFormat = "yyyy-MM-dd";
}

Frequency::Frequency(QObject* parent) : QObject(parent), MetaData() {
    setMetadata(Key::id, QUuid::createUuid());
    setMetadata(Key::frequency, OpenAccountEnums::Frequency::Once);
    setMetadata(Key::dateFormat, QString::fromLatin1(kDefaultDateFormat));
    setMetadata(Key::customIntervalDays, 1);
    setMetadata(Key::prototype, TransactionPtr{});
}

Frequency::Frequency(const QJsonObject& json, QObject* parent)
    : QObject(parent), MetaData() {
    // Seed defaults before overriding from JSON.
    setMetadata(Key::id, QUuid::createUuid());
    setMetadata(Key::frequency, OpenAccountEnums::Frequency::Once);
    setMetadata(Key::dateFormat, QString::fromLatin1(kDefaultDateFormat));
    setMetadata(Key::customIntervalDays, 1);
    setMetadata(Key::prototype, TransactionPtr{});
    Frequency::fromJson(json);
}

// --- Setters ---------------------------------------------------------------

void Frequency::setId(QUuid value) {
    if (id() == value) return;
    setMetadata(Key::id, value);
    emit idChanged();
    emit changed();
}

void Frequency::setFrequency(OpenAccountEnums::Frequency value) {
    if (frequency() == value) return;
    setMetadata(Key::frequency, value);
    emit frequencyChanged();
    emit changed();
}

void Frequency::setDateFormat(QString value) {
    if (value.isEmpty())
        value = QString::fromLatin1(kDefaultDateFormat);
    if (dateFormat() == value) return;
    setMetadata(Key::dateFormat, value);
    emit dateFormatChanged();
    emit changed();
}

void Frequency::setCustomIntervalDays(int value) {
    if (value < 1)
        value = 1;
    if (customIntervalDays() == value) return;
    setMetadata(Key::customIntervalDays, value);
    emit customIntervalDaysChanged();
    emit changed();
}

void Frequency::setPrototype(TransactionPtr value) {
    if (prototype() == value) return;
    setMetadata(Key::prototype, value);
    emit prototypeChanged();
    emit changed();
}

// --- JSON ------------------------------------------------------------------

QJsonObject Frequency::toJson() const {
    QJsonObject o;
    o.insert(Key::id, id().toString(QUuid::WithoutBraces));
    o.insert(Key::frequency, enumToJson(frequency()));
    o.insert(Key::dateFormat, dateFormat());
    o.insert(Key::customIntervalDays, customIntervalDays());
    o.insert(Key::prototype,
             prototype() ? prototype()->toJson() : QJsonObject{});
    return o;
}

void Frequency::fromJson(const QJsonObject& json) {
    if (json.contains(Key::id)) {
        const QUuid parsed = QUuid::fromString(json.value(Key::id).toString());
        if (!parsed.isNull())
            setId(parsed);
    }
    if (json.contains(Key::frequency))
        setFrequency(enumFromJson<OpenAccountEnums::Frequency>(
            json.value(Key::frequency), OpenAccountEnums::Frequency::Once));
    if (json.contains(Key::dateFormat))
        setDateFormat(json.value(Key::dateFormat).toString());
    if (json.contains(Key::customIntervalDays))
        setCustomIntervalDays(json.value(Key::customIntervalDays).toInt(1));
    if (json.contains(Key::prototype)) {
        const QJsonValue v = json.value(Key::prototype);
        if (v.isObject() && !v.toObject().isEmpty())
            setPrototype(TransactionPtr::create(v.toObject()));
    }
}

// --- Generation ------------------------------------------------------------

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
    TransactionPtr t = TransactionPtr::create(proto->toJson());
    t->setId(QUuid::createUuid());
    t->setDate(date);
    t->setDescription(buildDescription(proto->description(), date));
    t->setMetadata(TransactionKey::frequencyId, id());
    return t;
}

QString Frequency::buildDescription(const QString& base, QDate date) const {
    const QString formatted = date.toString(dateFormat());
    if (base.trimmed().isEmpty())
        return formatted;
    return QStringLiteral("%1 - %2").arg(base, formatted);
}

QDate Frequency::nextDate(QDate current) const {
    if (frequency() == OpenAccountEnums::Frequency::Custom)
        return current.addDays(customIntervalDays());
    return current.addDays(OpenAccountEnums::dayToNext(current, frequency()));
}
