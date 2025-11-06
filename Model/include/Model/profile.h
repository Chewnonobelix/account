#pragma once

#include <QObject>
#include <QString>
#include <QUuid>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QMetaType>
#include "metadata.h"

/*
 * Profile class
 * Inherits from QObject and MetaData.
 * Attributes stored via setMetadata() / metaData<T>().
 * Provides Q_PROPERTY bindings for firstName, lastName, and accounts.
 */

class Profile : public QObject, public MetaData
{
    Q_OBJECT

    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName  READ lastName  WRITE setLastName  NOTIFY lastNameChanged)
    Q_PROPERTY(QVector<QUuid> accounts READ accounts WRITE setAccounts NOTIFY accountsChanged)

public:
    explicit Profile(QObject *parent = nullptr);
    explicit Profile(const QJsonObject &obj, QObject *parent = nullptr);

    Profile(const Profile &other) = delete;
    Profile &operator=(const Profile &other) = delete;

    // Getters
    QUuid id() const;
    QString firstName() const;
    QString lastName() const;
    QVector<QUuid> accounts() const;

    // Setters
    void setId(QUuid id);
    void setFirstName(QString firstName);
    void setLastName(QString lastName);
    void setAccounts(QVector<QUuid> accounts);

    // Serialization
    QJsonObject toJson() const;

    // Integrity check
    bool isValid() const;

signals:
    void firstNameChanged();
    void lastNameChanged();
    void accountsChanged();
    void profileChanged();

private:
    static constexpr const char *KEY_ID = "id";
    static constexpr const char *KEY_FIRSTNAME = "firstName";
    static constexpr const char *KEY_LASTNAME = "lastName";
    static constexpr const char *KEY_ACCOUNTS = "accounts";

    static QJsonArray accountsToJsonArray(const QVector<QUuid> &accounts);
    static QVector<QUuid> jsonArrayToAccounts(const QJsonArray &array);
};

Q_DECLARE_METATYPE(QVector<QUuid>)
