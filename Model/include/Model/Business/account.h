#pragma once

#include "metadata.h"
#include "model_global.h"
#include <QDate>
#include <QEnableSharedFromThis>
#include <QJsonObject>
#include <QObject>
#include <QQmlEngine>
#include <QSharedPointer>
#include <QString>
#include <QUuid>

/**
 * @brief Bank account entity.
 *
 * Stores typed attributes through MetaData; exposes Qt properties for
 * QML integration. Emits a per-property change signal and a generic
 * `changed()` aggregate signal whenever any attribute is modified.
 */
class MODEL_EXPORT Account : public QObject,
                              public MetaData,
                              public QEnableSharedFromThis<Account> {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QUuid id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString number READ number WRITE setNumber NOTIFY numberChanged)
    Q_PROPERTY(QString bank READ bank WRITE setBank NOTIFY bankChanged)
    Q_PROPERTY(double interest READ interest WRITE setInterest NOTIFY interestChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QDate opening READ opening WRITE setOpening NOTIFY openingChanged)

public:
    explicit Account(QObject* parent = nullptr);
    explicit Account(const QJsonObject& json, QObject* parent = nullptr);
    ~Account() override = default;

    Account(const Account&) = delete;
    Account& operator=(const Account&) = delete;
    Account(Account&&) = delete;
    Account& operator=(Account&&) = delete;

    // Accessors
    [[nodiscard]] QUuid id() const { return metaData<QUuid>(Key::id); }
    [[nodiscard]] QString number() const { return metaData<QString>(Key::number); }
    [[nodiscard]] QString bank() const { return metaData<QString>(Key::bank); }
    [[nodiscard]] double interest() const { return metaData<double>(Key::interest); }
    [[nodiscard]] QString description() const { return metaData<QString>(Key::description); }
    [[nodiscard]] QDate opening() const { return metaData<QDate>(Key::opening); }

    // JSON I/O
    [[nodiscard]] QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

public slots:
    void setId(QUuid v);
    void setNumber(QString v);
    void setBank(QString v);
    void setInterest(double v);
    void setDescription(QString v);
    void setOpening(QDate v);

signals:
    void idChanged();
    void numberChanged();
    void bankChanged();
    void interestChanged();
    void descriptionChanged();
    void openingChanged();
    void changed();

private:
    struct Key {
        static constexpr auto id = "id";
        static constexpr auto number = "number";
        static constexpr auto bank = "bank";
        static constexpr auto interest = "interest";
        static constexpr auto description = "description";
        static constexpr auto opening = "opening";
    };
};

using AccountPtr = QSharedPointer<Account>;
Q_DECLARE_METATYPE(AccountPtr)
