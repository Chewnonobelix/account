#include "../include/Model/account.h"
#include <QSignalSpy>
#include <QtTest/QtTest>

class TestAccount : public QObject {
  Q_OBJECT
private slots:
  void test_defaultValues() {
    Account a;
    QCOMPARE(a.number(), QString());
    QCOMPARE(a.bank(), QString());
    QCOMPARE(a.interest(), 0.0);
    QCOMPARE(a.description(), QString());
    QCOMPARE(a.opening(), QDate());
    QVERIFY(a.id().isNull());
  }

  void test_settersGetters() {
    Account a;
    a.setNumber("FR761234567890");
    a.setBank("ACME Bank");
    a.setInterest(1.75);
    a.setDescription("Savings account");
    a.setOpening(QDate(2024, 6, 1));
    a.setId(QUuid::fromString("{3f2504e0-4f89-11d3-9a0c-0305e82c3301}"));

    QCOMPARE(a.number(), QString("FR761234567890"));
    QCOMPARE(a.bank(), QString("ACME Bank"));
    QCOMPARE(a.interest(), 1.75);
    QCOMPARE(a.description(), QString("Savings account"));
    QCOMPARE(a.opening(), QDate(2024, 6, 1));
    QCOMPARE(a.id(),
             QUuid::fromString("{3f2504e0-4f89-11d3-9a0c-0305e82c3301}"));
  }

  void test_jsonRoundTrip() {
    Account a;
    a.setNumber("FR00 0000 0000 0000");
    a.setBank("Bankii");
    a.setInterest(2.25);
    a.setDescription("Main account");
    a.setOpening(QDate(2023, 1, 31));
    a.setId(QUuid::createUuid());

    QJsonObject j = a.toJson();
    Account b(j);

    QCOMPARE(b.number(), a.number());
    QCOMPARE(b.bank(), a.bank());
    QCOMPARE(b.interest(), a.interest());
    QCOMPARE(b.description(), a.description());
    QCOMPARE(b.opening(), a.opening());
    QCOMPARE(b.id(), a.id());
  }

  void test_copyAndAssign() {
    Account a;
    a.setNumber("A");
    a.setBank("B");
    a.setInterest(3.14);
    a.setDescription("C");
    a.setOpening(QDate(2022, 12, 24));
    a.setId(QUuid::createUuid());

    Account b = a; // copy-ctor
    QCOMPARE(b.number(), a.number());
    QCOMPARE(b.bank(), a.bank());
    QCOMPARE(b.interest(), a.interest());
    QCOMPARE(b.description(), a.description());
    QCOMPARE(b.opening(), a.opening());
    QCOMPARE(b.id(), a.id());

    Account c; // copy-assign
    c = a;
    QCOMPARE(c.number(), a.number());
    QCOMPARE(c.bank(), a.bank());
    QCOMPARE(c.interest(), a.interest());
    QCOMPARE(c.description(), a.description());
    QCOMPARE(c.opening(), a.opening());
    QCOMPARE(c.id(), a.id());

    Account d = std::move(a); // move-ctor
    QCOMPARE(d.number(), b.number());
    QCOMPARE(d.bank(), b.bank());
    QCOMPARE(d.interest(), b.interest());
    QCOMPARE(d.description(), b.description());
    QCOMPARE(d.opening(), b.opening());
    QCOMPARE(d.id(), b.id());

    Account e; // move-assign
    e = std::move(b);
    QCOMPARE(e.number(), d.number());
    QCOMPARE(e.bank(), d.bank());
    QCOMPARE(e.interest(), d.interest());
    QCOMPARE(e.description(), d.description());
    QCOMPARE(e.opening(), d.opening());
    QCOMPARE(e.id(), d.id());
  }

  void test_signalEmissions_onChange() {
    Account a;

    QSignalSpy spyNumber(&a, SIGNAL(numberChanged()));
    a.setNumber("X");
    QCOMPARE(spyNumber.count(), 1);
    a.setNumber("X"); // no change
    QCOMPARE(spyNumber.count(), 1);

    QSignalSpy spyBank(&a, SIGNAL(bankChanged()));
    a.setBank("BankA");
    QCOMPARE(spyBank.count(), 1);
    a.setBank("BankA");
    QCOMPARE(spyBank.count(), 1);

    QSignalSpy spyInterest(&a, SIGNAL(interestChanged()));
    a.setInterest(1.0);
    QCOMPARE(spyInterest.count(), 1);
    a.setInterest(1.0);
    QCOMPARE(spyInterest.count(), 1);

    QSignalSpy spyDescription(&a, SIGNAL(descriptionChanged()));
    a.setDescription("Desc");
    QCOMPARE(spyDescription.count(), 1);
    a.setDescription("Desc");
    QCOMPARE(spyDescription.count(), 1);

    QSignalSpy spyOpening(&a, SIGNAL(openingChanged()));
    a.setOpening(QDate(2024, 1, 1));
    QCOMPARE(spyOpening.count(), 1);
    a.setOpening(QDate(2024, 1, 1));
    QCOMPARE(spyOpening.count(), 1);
  }

  void test_changedSignal_broadcast() {
    Account a;
    QSignalSpy spyChanged(&a, SIGNAL(changed()));

    a.setNumber("N1");
    a.setBank("B1");
    a.setInterest(2.5);
    a.setDescription("D1");
    a.setOpening(QDate(2023, 5, 20));

    QCOMPARE(spyChanged.count(), 5);

    a.setNumber("N1");
    a.setBank("B1");
    a.setInterest(2.5);
    a.setDescription("D1");
    a.setOpening(QDate(2023, 5, 20));
    QCOMPARE(spyChanged.count(), 5);
  }
};

QTEST_MAIN(TestAccount)
#include "tst_account.moc"
