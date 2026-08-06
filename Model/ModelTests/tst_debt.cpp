#include "../include/Model/Business/debt.h"

#include <QSignalSpy>
#include <QtTest/QtTest>

class TestDebt : public QObject {
  Q_OBJECT

private slots:
  void defaults_are_sane() {
    Debt debt;

    QVERIFY(!debt.id().isNull());
    QCOMPARE(debt.name(), QString());
    QCOMPARE(debt.counterparty(), QString());
    QCOMPARE(debt.principal(), 0.0);
    QCOMPARE(debt.remaining(), 0.0);
    QCOMPARE(debt.interest(), 0.0);
    QCOMPARE(debt.startDate(), QDate());
    QCOMPARE(debt.endDate(), QDate());
    QCOMPARE(debt.recurrence(), OpenAccountEnums::Frequency::Once);
    QCOMPARE(debt.direction(), OpenAccountEnums::Movement::Debit);
    QCOMPARE(debt.description(), QString());
    QVERIFY(debt.accountId().isNull());
    QVERIFY(debt.isClosed());
  }

  void setters_apply_guards() {
    Debt debt;

    debt.setPrincipal(1000.0);
    debt.setRemaining(1200.0);
    debt.setInterest(-3.0);
    debt.setRecurrence(OpenAccountEnums::Frequency::Custom);
    debt.setDirection(OpenAccountEnums::Movement::Both);

    QCOMPARE(debt.principal(), 1000.0);
    QCOMPARE(debt.remaining(), 1000.0);
    QCOMPARE(debt.interest(), 0.0);
    QCOMPARE(debt.recurrence(), OpenAccountEnums::Frequency::Once);
    QCOMPARE(debt.direction(), OpenAccountEnums::Movement::Debit);

    debt.setRemaining(250.0);
    debt.setPrincipal(100.0);
    QCOMPARE(debt.remaining(), 100.0);
  }

  void json_roundtrip_restores_values() {
    Debt debt;
    const QUuid id = QUuid::createUuid();
    const QUuid accountId = QUuid::createUuid();

    debt.setId(id);
    debt.setName(QStringLiteral("Pret voiture"));
    debt.setCounterparty(QStringLiteral("Banque Demo"));
    debt.setPrincipal(12500.0);
    debt.setRemaining(9300.5);
    debt.setInterest(2.35);
    debt.setStartDate(QDate(2026, 1, 12));
    debt.setEndDate(QDate(2030, 1, 12));
    debt.setRecurrence(OpenAccountEnums::Frequency::Monthly);
    debt.setDirection(OpenAccountEnums::Movement::Credit);
    debt.setDescription(QStringLiteral("Pret familial"));
    debt.setAccountId(accountId);

    const Debt restored(debt.toJson());

    QCOMPARE(restored.id(), id);
    QCOMPARE(restored.name(), debt.name());
    QCOMPARE(restored.counterparty(), debt.counterparty());
    QCOMPARE(restored.principal(), debt.principal());
    QCOMPARE(restored.remaining(), debt.remaining());
    QCOMPARE(restored.interest(), debt.interest());
    QCOMPARE(restored.startDate(), debt.startDate());
    QCOMPARE(restored.endDate(), debt.endDate());
    QCOMPARE(restored.recurrence(), debt.recurrence());
    QCOMPARE(restored.direction(), debt.direction());
    QCOMPARE(restored.description(), debt.description());
    QCOMPARE(restored.accountId(), debt.accountId());
  }

  void signals_fire_once_per_effective_change() {
    Debt debt;
    QSignalSpy changedSpy(&debt, &Debt::changed);
    QSignalSpy nameSpy(&debt, &Debt::nameChanged);
    QSignalSpy remainingSpy(&debt, &Debt::remainingChanged);

    debt.setName(QStringLiteral("Pret"));
    debt.setName(QStringLiteral("Pret"));
    QCOMPARE(nameSpy.count(), 1);

    debt.setPrincipal(500.0);
    debt.setRemaining(300.0);
    debt.setRemaining(300.0);
    QCOMPARE(remainingSpy.count(), 2);

    QCOMPARE(changedSpy.count(), 3);
  }

  void generateRepayments_usesFrequencyDatesAndAmount() {
    Debt debt;
    const QUuid accountId = QUuid::createUuid();

    debt.setName(QStringLiteral("Pret voiture"));
    debt.setPrincipal(100.0);
    debt.setInterest(20.0);
    debt.setStartDate(QDate(2026, 1, 1));
    debt.setEndDate(QDate(2026, 3, 1));
    debt.setRecurrence(OpenAccountEnums::Frequency::Monthly);
    debt.setAccountId(accountId);

    const QList<TransactionPtr> repayments = debt.generateRepayments();

    QCOMPARE(repayments.size(), 3);
    QCOMPARE(repayments.at(0)->date(), QDate(2026, 1, 1));
    QCOMPARE(repayments.at(1)->date(), QDate(2026, 2, 1));
    QCOMPARE(repayments.at(2)->date(), QDate(2026, 3, 1));
    QCOMPARE(repayments.at(0)->value(), 40.0);
    QCOMPARE(repayments.at(1)->value(), 40.0);
    QCOMPARE(repayments.at(2)->value(), 40.0);
    QCOMPARE(repayments.at(0)->movement(), OpenAccountEnums::Movement::Debit);
    QCOMPARE(repayments.at(0)->accountId(), accountId);
    QCOMPARE(repayments.at(0)->description(),
             QStringLiteral("Pret voiture - 2026-01-01"));
    QCOMPARE(repayments.at(0)->metaData<QUuid>(QStringLiteral("frequencyId")),
             debt.id());
    QCOMPARE(repayments.at(0)->metaData<QUuid>(QStringLiteral("debtId")),
             debt.id());
  }

  void generateRepayments_distributesRoundingOnFirstPayments() {
    Debt debt;

    debt.setDescription(QStringLiteral("Remboursement"));
    debt.setPrincipal(100.0);
    debt.setInterest(10.0);
    debt.setStartDate(QDate(2026, 1, 1));
    debt.setEndDate(QDate(2026, 1, 3));
    debt.setRecurrence(OpenAccountEnums::Frequency::Daily);
    debt.setDirection(OpenAccountEnums::Movement::Credit);

    const QList<TransactionPtr> repayments = debt.generateRepayments();

    QCOMPARE(repayments.size(), 3);
    QCOMPARE(repayments.at(0)->value(), 36.67);
    QCOMPARE(repayments.at(1)->value(), 36.67);
    QCOMPARE(repayments.at(2)->value(), 36.66);
    QCOMPARE(repayments.at(0)->movement(), OpenAccountEnums::Movement::Credit);
    QCOMPARE(repayments.at(0)->description(),
             QStringLiteral("Remboursement - 2026-01-01"));
  }

  void generateRepayments_invalidConfigurationReturnsEmpty() {
    Debt debt;

    QCOMPARE(debt.generateRepayments().size(), 0);

    debt.setPrincipal(100.0);
    debt.setStartDate(QDate(2026, 2, 1));
    debt.setEndDate(QDate(2026, 1, 1));

    QCOMPARE(debt.generateRepayments().size(), 0);
  }
};

QTEST_MAIN(TestDebt)
#include "tst_debt.moc"
