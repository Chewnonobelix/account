#pragma once

#include "enums.h"
#include "metadata.h"
#include "model_global.h"
#include <QDate>
#include <QEnableSharedFromThis>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QUuid>

class Total;       // forward declaration
class Transaction; // forward declaration

/**
 * @brief Monetary operation attached to an Account and optionally a Category.
 *
 * Inherits QEnableSharedFromThis privately (C++ Core Guideline C.129) —
 * sharedFromThis() remains accessible via `this->sharedFromThis()` inside
 * Transaction and is granted to friend operators building Totals.
 */
class MODEL_EXPORT Transaction
    : public QObject,
      public MetaData,
      private QEnableSharedFromThis<Transaction> {
    Q_OBJECT

    Q_PROPERTY(QUuid id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(OpenAccountEnums::Support support READ support WRITE setSupport NOTIFY supportChanged)
    Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(OpenAccountEnums::Movement movement READ movement WRITE setMovement NOTIFY movementChanged)
    Q_PROPERTY(bool isVisible READ isVisible WRITE setIsVisible NOTIFY isVisibleChanged)
    Q_PROPERTY(QUuid accountId READ accountId WRITE setAccountId NOTIFY accountIdChanged)
    Q_PROPERTY(QUuid category READ category WRITE setCategory NOTIFY categoryChanged)

public:
    explicit Transaction(QObject* parent = nullptr);
    explicit Transaction(const QJsonObject& json, QObject* parent = nullptr);
    ~Transaction() override = default;

    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;
    Transaction(Transaction&&) = delete;
    Transaction& operator=(Transaction&&) = delete;

    using QEnableSharedFromThis<Transaction>::sharedFromThis;

    // Accessors
    [[nodiscard]] QUuid id() const { return metaData<QUuid>(Key::id); }
    [[nodiscard]] double value() const { return metaData<double>(Key::value); }
    [[nodiscard]] QString description() const { return metaData<QString>(Key::description); }
    [[nodiscard]] OpenAccountEnums::Support support() const {
        return metaData<OpenAccountEnums::Support>(Key::support);
    }
    [[nodiscard]] QDate date() const { return metaData<QDate>(Key::date); }
    [[nodiscard]] OpenAccountEnums::Movement movement() const {
        return metaData<OpenAccountEnums::Movement>(Key::movement);
    }
    [[nodiscard]] bool isVisible() const { return metaData<bool>(Key::isVisible); }
    [[nodiscard]] QUuid accountId() const { return metaData<QUuid>(Key::accountId); }
    [[nodiscard]] QUuid category() const { return metaData<QUuid>(Key::category); }

    [[nodiscard]] QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

public slots:
    void setId(QUuid v);
    void setValue(double v);
    void setDescription(QString v);
    void setSupport(OpenAccountEnums::Support v);
    void setDate(QDate v);
    void setMovement(OpenAccountEnums::Movement v);
    void setIsVisible(bool v);
    void setAccountId(QUuid v);
    void setCategory(QUuid v);

signals:
    void idChanged();
    void valueChanged();
    void descriptionChanged();
    void supportChanged();
    void dateChanged();
    void movementChanged();
    void isVisibleChanged();
    void accountIdChanged();
    void categoryChanged();
    void changed();

    // Operators building/merging Totals
    friend Total operator+(Transaction& a, Transaction& b);
    friend Total operator+(Transaction& a, const Total& b);

private:
    struct Key {
        static constexpr auto id = "id";
        static constexpr auto value = "value";
        static constexpr auto description = "description";
        static constexpr auto support = "support";
        static constexpr auto date = "date";
        static constexpr auto movement = "movement";
        static constexpr auto isVisible = "isVisible";
        static constexpr auto accountId = "accountId";
        static constexpr auto category = "category";
    };
};

using TransactionPtr = QSharedPointer<Transaction>;
Q_DECLARE_METATYPE(TransactionPtr)
