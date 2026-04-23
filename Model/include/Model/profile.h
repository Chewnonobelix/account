#pragma once

#include "metadata.h"
#include "model_global.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QMetaType>
#include <QObject>
#include <QString>
#include <QUuid>

/**
 * @brief User profile owning a list of Account ids.
 */
class MODEL_EXPORT Profile : public QObject, public MetaData {
    Q_OBJECT

    Q_PROPERTY(QUuid id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QList<QUuid> accounts READ accounts WRITE setAccounts NOTIFY accountsChanged)

public:
    explicit Profile(QObject* parent = nullptr);
    explicit Profile(const QJsonObject& obj, QObject* parent = nullptr);
    ~Profile() override = default;

    Profile(const Profile&) = delete;
    Profile& operator=(const Profile&) = delete;
    Profile(Profile&&) = delete;
    Profile& operator=(Profile&&) = delete;

    [[nodiscard]] QUuid id() const { return metaData<QUuid>(Key::id); }
    [[nodiscard]] QString firstName() const { return metaData<QString>(Key::firstName); }
    [[nodiscard]] QString lastName() const { return metaData<QString>(Key::lastName); }
    [[nodiscard]] QList<QUuid> accounts() const { return metaData<QList<QUuid>>(Key::accounts); }

    [[nodiscard]] QJsonObject toJson() const override;
    void fromJson(const QJsonObject& obj) override;

    /** True when id is non-null and last name is set. */
    [[nodiscard]] bool isValid() const;

public slots:
    void setId(QUuid id);
    void setFirstName(QString firstName);
    void setLastName(QString lastName);
    void setAccounts(QList<QUuid> accounts);

signals:
    void idChanged();
    void firstNameChanged();
    void lastNameChanged();
    void accountsChanged();
    void changed();

private:
    struct Key {
        static constexpr auto id = "id";
        static constexpr auto firstName = "firstName";
        static constexpr auto lastName = "lastName";
        static constexpr auto accounts = "accounts";
    };

    static QJsonArray accountsToJson(const QList<QUuid>& accounts);
    static QList<QUuid> accountsFromJson(const QJsonArray& arr);
};

Q_DECLARE_METATYPE(QList<QUuid>)
