#include "../include/Model/Business/account.h"
#include <QSignalSpy>
#include <QtTest>

/*
 * Unit tests for Account
 * Tests default values, setters/getters, JSON round-trip and signals.
 * Account is deliberately non-copyable and non-movable (QObject identity),
 * so copy/move semantics are not exercised.
 */

class TestAccount : public QObject {
    Q_OBJECT

private slots:
    void testDefaultValues();
    void testSetGet();
    void testJsonRoundTrip();
    void testSignalEmissions();
    void testChangedBroadcast();
};

void TestAccount::testDefaultValues() {
    Account a;
    QCOMPARE(a.number(), QString());
    QCOMPARE(a.bank(), QString());
    QCOMPARE(a.interest(), 0.0);
    QCOMPARE(a.description(), QString());
    QCOMPARE(a.opening(), QDate());
    QVERIFY(a.id().isNull());
}

void TestAccount::testSetGet() {
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

void TestAccount::testJsonRoundTrip() {
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

void TestAccount::testSignalEmissions() {
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

void TestAccount::testChangedBroadcast() {
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

QTEST_MAIN(TestAccount)
#include "tst_account.moc"
