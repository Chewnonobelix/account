#pragma once

#include "enums.h"
#include "metadata.h"
#include "model_global.h"
#include "transaction.h"

#include <QDate>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QString>
#include <QUuid>

/**
 * @brief Stores either a debt or a granted loan.
 *
 * direction() == Debit means money owed, Credit means money lent.
 */
class MODEL_EXPORT Debt : public QObject, public MetaData {
    Q_OBJECT

    Q_PROPERTY(QUuid id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString counterparty READ counterparty WRITE setCounterparty NOTIFY counterpartyChanged)
    Q_PROPERTY(double principal READ principal WRITE setPrincipal NOTIFY principalChanged)
    Q_PROPERTY(double remaining READ remaining WRITE setRemaining NOTIFY remainingChanged)
    Q_PROPERTY(double interest READ interest WRITE setInterest NOTIFY interestChanged)
    Q_PROPERTY(QDate startDate READ startDate WRITE setStartDate NOTIFY startDateChanged)
    Q_PROPERTY(QDate endDate READ endDate WRITE setEndDate NOTIFY endDateChanged)
    Q_PROPERTY(OpenAccountEnums::Frequency recurrence READ recurrence WRITE setRecurrence NOTIFY recurrenceChanged)
    Q_PROPERTY(OpenAccountEnums::Movement direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QUuid accountId READ accountId WRITE setAccountId NOTIFY accountIdChanged)
    Q_PROPERTY(bool closed READ isClosed NOTIFY remainingChanged)

public:
    explicit Debt(QObject* parent = nullptr);
    explicit Debt(const QJsonObject& json, QObject* parent = nullptr);
    ~Debt() override = default;

    Debt(const Debt&) = delete;
    Debt& operator=(const Debt&) = delete;
    Debt(Debt&&) = delete;
    Debt& operator=(Debt&&) = delete;

    // Accessors
    [[nodiscard]] QUuid id() const { return metaData<QUuid>(Key::id); }
    [[nodiscard]] QString name() const { return metaData<QString>(Key::name); }
    [[nodiscard]] QString counterparty() const {
        return metaData<QString>(Key::counterparty);
    }
    [[nodiscard]] double principal() const { return metaData<double>(Key::principal); }
    [[nodiscard]] double remaining() const { return metaData<double>(Key::remaining); }
    [[nodiscard]] double interest() const { return metaData<double>(Key::interest); }
    [[nodiscard]] QDate startDate() const { return metaData<QDate>(Key::startDate); }
    [[nodiscard]] QDate endDate() const { return metaData<QDate>(Key::endDate); }
    [[nodiscard]] OpenAccountEnums::Frequency recurrence() const {
        return metaData<OpenAccountEnums::Frequency>(Key::recurrence);
    }
    [[nodiscard]] OpenAccountEnums::Movement direction() const {
        return metaData<OpenAccountEnums::Movement>(Key::direction);
    }
    [[nodiscard]] QString description() const {
        return metaData<QString>(Key::description);
    }
    [[nodiscard]] QUuid accountId() const { return metaData<QUuid>(Key::accountId); }
    [[nodiscard]] bool isClosed() const { return remaining() <= 0.0; }

    [[nodiscard]] QList<TransactionPtr> generateRepayments() const;

    [[nodiscard]] QJsonObject toJson() const override;
    void fromJson(const QJsonObject& json) override;

public slots:
    void setId(QUuid value);
    void setName(QString value);
    void setCounterparty(QString value);
    void setPrincipal(double value);
    void setRemaining(double value);
    void setInterest(double value);
    void setStartDate(QDate value);
    void setEndDate(QDate value);
    void setRecurrence(OpenAccountEnums::Frequency value);
    void setDirection(OpenAccountEnums::Movement value);
    void setDescription(QString value);
    void setAccountId(QUuid value);

signals:
    void idChanged();
    void nameChanged();
    void counterpartyChanged();
    void principalChanged();
    void remainingChanged();
    void interestChanged();
    void startDateChanged();
    void endDateChanged();
    void recurrenceChanged();
    void directionChanged();
    void descriptionChanged();
    void accountIdChanged();
    void changed();

private:
    struct Key {
        static constexpr auto id = "id";
        static constexpr auto name = "name";
        static constexpr auto counterparty = "counterparty";
        static constexpr auto principal = "principal";
        static constexpr auto remaining = "remaining";
        static constexpr auto interest = "interest";
        static constexpr auto startDate = "startDate";
        static constexpr auto endDate = "endDate";
        static constexpr auto recurrence = "recurrence";
        static constexpr auto direction = "direction";
        static constexpr auto description = "description";
        static constexpr auto accountId = "accountId";
    };
};
