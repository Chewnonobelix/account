#include "Model\transaction.h"
#include <QJsonValue>

Transaction::Transaction(QObject* parent)
    : QObject(parent)
{
    // Defaults
    setMetadata(Key::Id, QUuid::createUuid());
    setMetadata(Key::Value, 0.0);
    setMetadata(Key::Description, QString());
    setMetadata(Key::Support, OpenAccountEnums::Support{});
    setMetadata(Key::Date, QDate::currentDate());
    setMetadata(Key::Movement, OpenAccountEnums::Movement{});
    setMetadata(Key::IsVisible, true);
    setMetadata(Key::AccountId, QUuid());
    setMetadata(Key::Category, QString());
}

Transaction::Transaction(const QJsonObject& json, QObject* parent)
    : QObject(parent)
{
    // Initialize defaults then override with JSON
    setMetadata(Key::Id, QUuid::createUuid());
    setMetadata(Key::Value, 0.0);
    setMetadata(Key::Description, QString());
    setMetadata(Key::Support, OpenAccountEnums::Support{});
    setMetadata(Key::Date, QDate::currentDate());
    setMetadata(Key::Movement, OpenAccountEnums::Movement{});
    setMetadata(Key::IsVisible, true);
    setMetadata(Key::AccountId, QUuid());
    setMetadata(Key::Category, QString());

    if (json.contains(Key::Id))         setMetadata(Key::Id, QUuid(json.value(Key::Id).toString()));
    if (json.contains(Key::Value))      setMetadata(Key::Value, json.value(Key::Value).toDouble());
    if (json.contains(Key::Description))setMetadata(Key::Description, json.value(Key::Description).toString());
    if (json.contains(Key::Support))    setMetadata(Key::Support, static_cast<OpenAccountEnums::Support>(json.value(Key::Support).toInt()));
    if (json.contains(Key::Date))       setMetadata(Key::Date, QDate::fromString(json.value(Key::Date).toString(), Qt::ISODate));
    if (json.contains(Key::Movement))   setMetadata(Key::Movement, static_cast<OpenAccountEnums::Movement>(json.value(Key::Movement).toInt()));
    if (json.contains(Key::IsVisible))  setMetadata(Key::IsVisible, json.value(Key::IsVisible).toBool());
    if (json.contains(Key::AccountId))  setMetadata(Key::AccountId, QUuid(json.value(Key::AccountId).toString()));
    if (json.contains(Key::Category))   setMetadata(Key::Category, json.value(Key::Category).toString());
}

// Setters
void Transaction::setId(QUuid v)                            { assignIfChanged(Key::Id, v, &Transaction::idChanged); }
void Transaction::setValue(double v)                        { assignIfChanged(Key::Value, v, &Transaction::valueChanged); }
void Transaction::setDescription(QString v)                 { assignIfChanged(Key::Description, v, &Transaction::descriptionChanged); }
void Transaction::setSupport(OpenAccountEnums::Support v)   { assignIfChanged(Key::Support, v, &Transaction::supportChanged); }
void Transaction::setDate(QDate v)                          { assignIfChanged(Key::Date, v, &Transaction::dateChanged); }
void Transaction::setMovement(OpenAccountEnums::Movement v) { assignIfChanged(Key::Movement, v, &Transaction::movementChanged); }
void Transaction::setIsVisible(bool v)                      { assignIfChanged(Key::IsVisible, v, &Transaction::isVisibleChanged); }
void Transaction::setAccountId(QUuid v)                     { assignIfChanged(Key::AccountId, v, &Transaction::accountIdChanged); }
void Transaction::setCategory(QString v)                    { assignIfChanged(Key::Category, v, &Transaction::categoryChanged); }

QJsonObject Transaction::toJson() const
{
    QJsonObject o;
    o.insert(Key::Id, id().toString(QUuid::WithoutBraces));
    o.insert(Key::Value, value());
    o.insert(Key::Description, description());
    o.insert(Key::Support, static_cast<int>(support()));
    o.insert(Key::Date, date().toString(Qt::ISODate));
    o.insert(Key::Movement, static_cast<int>(movement()));
    o.insert(Key::IsVisible, isVisible());
    o.insert(Key::AccountId, accountId().toString(QUuid::WithoutBraces));
    o.insert(Key::Category, category());
    return o;
}
