#pragma once

#include "enums.h"
#include "metadata.h"
#include "model_global.h"
#include <QEnableSharedFromThis>
#include <QJsonObject>
#include <QObject>
#include <QQmlEngine>
#include <QSharedPointer>
#include <QString>
#include <QUuid>

/**
 * @brief Labels transactions (e.g. "Food", "Salary") and declares a direction.
 */
class MODEL_EXPORT Category : public QObject,
                               public MetaData,
                               public QEnableSharedFromThis<Category> {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QUuid id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(OpenAccountEnums::Movement direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit Category(QObject* parent = nullptr);
    explicit Category(const QJsonObject& json, QObject* parent = nullptr);
    ~Category() override = default;

    Category(const Category&) = delete;
    Category& operator=(const Category&) = delete;
    Category(Category&&) = delete;
    Category& operator=(Category&&) = delete;

    [[nodiscard]] QUuid id() const { return metaData<QUuid>(Key::id); }
    [[nodiscard]] OpenAccountEnums::Movement direction() const {
        return metaData<OpenAccountEnums::Movement>(Key::direction);
    }
    [[nodiscard]] QString name() const { return metaData<QString>(Key::name); }

    [[nodiscard]] QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

public slots:
    void setId(QUuid id);
    void setDirection(OpenAccountEnums::Movement direction);
    void setName(QString name);

signals:
    void idChanged();
    void directionChanged();
    void nameChanged();
    void changed();

private:
    struct Key {
        static constexpr auto id = "id";
        static constexpr auto direction = "direction";
        static constexpr auto name = "name";
    };
};

using CategoryPtr = QSharedPointer<Category>;
Q_DECLARE_METATYPE(CategoryPtr)
