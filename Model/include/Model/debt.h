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

/*
 * Debt stores either a debt or a granted loan.
 * direction() == Debit means money owed, Credit means money lent.
 */
class MODEL_EXPORT Debt : public QObject, public MetaData {
  Q_OBJECT

  Q_PROPERTY(QUuid id READ id NOTIFY idChanged)
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  Q_PROPERTY(QString counterparty READ counterparty WRITE setCounterparty NOTIFY
                 counterpartyChanged)
  Q_PROPERTY(double principal READ principal WRITE setPrincipal NOTIFY
                 principalChanged)
  Q_PROPERTY(double remaining READ remaining WRITE setRemaining NOTIFY
                 remainingChanged)
  Q_PROPERTY(double interest READ interest WRITE setInterest NOTIFY
                 interestChanged)
  Q_PROPERTY(QDate startDate READ startDate WRITE setStartDate NOTIFY
                 startDateChanged)
  Q_PROPERTY(
      QDate endDate READ endDate WRITE setEndDate NOTIFY endDateChanged)
  Q_PROPERTY(OpenAccountEnums::Frequency recurrence READ recurrence WRITE
                 setRecurrence NOTIFY recurrenceChanged)
  Q_PROPERTY(OpenAccountEnums::Movement direction READ direction WRITE
                 setDirection NOTIFY directionChanged)
  Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY
                 descriptionChanged)
  Q_PROPERTY(
      QUuid accountId READ accountId WRITE setAccountId NOTIFY accountIdChanged)
  Q_PROPERTY(bool closed READ isClosed NOTIFY remainingChanged)

public:
  explicit Debt(QObject *parent = nullptr);
  explicit Debt(const QJsonObject &json, QObject *parent = nullptr);

  Debt(const Debt &) = delete;
  Debt &operator=(const Debt &) = delete;
  ~Debt() override;

  QUuid id() const;
  QString name() const;
  QString counterparty() const;
  double principal() const;
  double remaining() const;
  double interest() const;
  QDate startDate() const;
  QDate endDate() const;
  OpenAccountEnums::Frequency recurrence() const;
  OpenAccountEnums::Movement direction() const;
  QString description() const;
  QUuid accountId() const;
  bool isClosed() const;

  QList<TransactionPtr> generateRepayments() const;

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
  void changed();
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

public:
  QJsonObject toJson() const;

private:
  struct Key {
    static constexpr auto Id = "id";
    static constexpr auto Name = "name";
    static constexpr auto Counterparty = "counterparty";
    static constexpr auto Principal = "principal";
    static constexpr auto Remaining = "remaining";
    static constexpr auto Interest = "interest";
    static constexpr auto StartDate = "startDate";
    static constexpr auto EndDate = "endDate";
    static constexpr auto Recurrence = "recurrence";
    static constexpr auto Direction = "direction";
    static constexpr auto Description = "description";
    static constexpr auto AccountId = "accountId";
  };
};
