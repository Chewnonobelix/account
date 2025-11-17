#pragma once

#include <QObject>
#include <QUuid>
#include <QString>
#include <QJsonObject>
#include <QMetaEnum>

#include "model_global.h"
#include "metadata.h"
#include "enums.h"

class MODEL_EXPORT Category : public QObject, public MetaData
{
    Q_OBJECT

    Q_PROPERTY(QUuid id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(OpenAccountEnums::Movement direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit Category(QObject *parent = nullptr);
    explicit Category(const QJsonObject &object, QObject *parent = nullptr);
    ~Category() override = default;

    Category(const Category &) = delete;
    Category &operator=(const Category &) = delete;

    QUuid id() const;
    void setId(QUuid id);

    OpenAccountEnums::Movement direction() const;
    void setDirection(OpenAccountEnums::Movement direction);

    QString name() const;
    void setName(QString name);

    QJsonObject toJson() const;

signals:
    void idChanged();
    void directionChanged();
    void nameChanged();

private:
    struct Key
    {
        static constexpr auto id = "id";
        static constexpr auto direction = "direction";
        static constexpr auto name = "name";
    };
};
