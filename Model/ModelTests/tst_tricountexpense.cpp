#include "../include/Model/tricountexpense.h"
#include "../include/Model/transaction.h"
#include <QSignalSpy>
#include <QtTest>

/*
 * Unit tests for TricountExpense
 * Tests setters/getters, amount proxying, participant/exclusion resolution,
 * cent-accurate equal split, JSON round-trip, validity, signals.
 */

class TestTricountExpense : public QObject {
    Q_OBJECT

private slots:
    void testSetGet();
    void testAmountFromTransaction();
    void testValidity();
    void testParticipantsExcludes();
    void testParticipantsFallbackToPayer();
    void testShareEqualSplit();
    void testShareCentAccurate();
    void testShareWithExclusion();
    void testJsonRoundTrip();
    void testSignal_excludedChanged();
    void testSignal_changedCounts();

private:
    static TransactionPtr makeTransaction(double value) {
        auto t = TransactionPtr::create();
        t->setValue(value);
        return t;
    }
    static double sum(const QMap<QUuid, double>& m) {
        double s = 0.0;
        for (double v : m) s += v;
        return s;
    }
};

void TestTricountExpense::testSetGet() {
    TricountExpense e;
    const QUuid id = QUuid::createUuid();
    const QUuid payer = QUuid::createUuid();
    const QList<QUuid> excluded = {QUuid::createUuid()};
    auto tx = makeTransaction(42.0);

    e.setId(id);
    e.setTransaction(tx);
    e.setPayerId(payer);
    e.setExcluded(excluded);

    QCOMPARE(e.id(), id);
    QCOMPARE(e.transaction(), tx);
    QCOMPARE(e.payerId(), payer);
    QCOMPARE(e.excluded(), excluded);
}

void TestTricountExpense::testAmountFromTransaction() {
    TricountExpense e;
    e.setTransaction(makeTransaction(-15.5)); // sign-agnostic
    QCOMPARE(e.amount(), 15.5);
}

void TestTricountExpense::testValidity() {
    TricountExpense e;
    QVERIFY(!e.isValid()); // no transaction, no payer

    e.setTransaction(makeTransaction(10.0));
    QVERIFY(!e.isValid()); // still no payer

    e.setPayerId(QUuid::createUuid());
    QVERIFY(e.isValid());

    e.setTransaction(makeTransaction(0.0));
    QVERIFY(!e.isValid()); // amount must be > 0
}

void TestTricountExpense::testParticipantsExcludes() {
    const QUuid a = QUuid::createUuid();
    const QUuid b = QUuid::createUuid();
    const QUuid c = QUuid::createUuid();

    TricountExpense e;
    e.setTransaction(makeTransaction(30.0));
    e.setPayerId(a);
    e.setExcluded({b});

    const QList<QUuid> got = e.participants({a, b, c});
    QCOMPARE(got, QList<QUuid>({a, c}));
}

void TestTricountExpense::testParticipantsFallbackToPayer() {
    const QUuid a = QUuid::createUuid();
    const QUuid b = QUuid::createUuid();

    TricountExpense e;
    e.setTransaction(makeTransaction(30.0));
    e.setPayerId(a);
    e.setExcluded({a, b}); // everyone excluded

    const QList<QUuid> got = e.participants({a, b});
    QCOMPARE(got, QList<QUuid>({a}));
}

void TestTricountExpense::testShareEqualSplit() {
    const QUuid a = QUuid::createUuid();
    const QUuid b = QUuid::createUuid();

    TricountExpense e;
    e.setTransaction(makeTransaction(30.0));
    e.setPayerId(a);

    const auto shares = e.shareByMember({a, b});
    QCOMPARE(shares.size(), 2);
    QCOMPARE(shares.value(a), 15.0);
    QCOMPARE(shares.value(b), 15.0);
    QVERIFY(qFuzzyCompare(sum(shares), 30.0));
}

void TestTricountExpense::testShareCentAccurate() {
    const QUuid a = QUuid::createUuid();
    const QUuid b = QUuid::createUuid();
    const QUuid c = QUuid::createUuid();

    TricountExpense e;
    e.setTransaction(makeTransaction(10.0)); // 10.00 / 3
    e.setPayerId(a);

    const auto shares = e.shareByMember({a, b, c});
    QCOMPARE(shares.size(), 3);
    QVERIFY(qFuzzyCompare(sum(shares), 10.0));
    for (double v : shares) {
        // each share is 3.33 or 3.34
        QVERIFY(qRound64(v * 100.0) == 333 || qRound64(v * 100.0) == 334);
    }
}

void TestTricountExpense::testShareWithExclusion() {
    const QUuid a = QUuid::createUuid();
    const QUuid b = QUuid::createUuid();
    const QUuid c = QUuid::createUuid();

    TricountExpense e;
    e.setTransaction(makeTransaction(30.0));
    e.setPayerId(a);
    e.setExcluded({c});

    const auto shares = e.shareByMember({a, b, c});
    QCOMPARE(shares.size(), 2);
    QVERIFY(!shares.contains(c));
    QCOMPARE(shares.value(a), 15.0);
    QCOMPARE(shares.value(b), 15.0);
}

void TestTricountExpense::testJsonRoundTrip() {
    TricountExpense e;
    e.setId(QUuid::createUuid());
    auto tx = makeTransaction(19.99);
    tx->setDescription("Restaurant");
    tx->setSupport(OpenAccountEnums::Support::Card);
    e.setTransaction(tx);
    e.setPayerId(QUuid::createUuid());
    e.setExcluded({QUuid::createUuid(), QUuid::createUuid()});

    const QJsonObject obj = e.toJson();
    TricountExpense copy(obj);

    QCOMPARE(copy.id(), e.id());
    QCOMPARE(copy.payerId(), e.payerId());
    QCOMPARE(copy.excluded(), e.excluded());
    QVERIFY(!copy.transaction().isNull());
    QCOMPARE(copy.transaction()->value(), e.transaction()->value());
    QCOMPARE(copy.transaction()->description(), QString("Restaurant"));
    QCOMPARE(copy.transaction()->support(), OpenAccountEnums::Support::Card);
}

void TestTricountExpense::testSignal_excludedChanged() {
    TricountExpense e;
    QSignalSpy spy(&e, SIGNAL(excludedChanged()));
    e.setExcluded({QUuid::createUuid()});
    QCOMPARE(spy.count(), 1);
}

void TestTricountExpense::testSignal_changedCounts() {
    TricountExpense e;
    QSignalSpy spy(&e, SIGNAL(changed()));
    e.setTransaction(makeTransaction(5.0));
    e.setPayerId(QUuid::createUuid());
    e.setExcluded({QUuid::createUuid()});
    QVERIFY(spy.count() >= 3);
}

QTEST_MAIN(TestTricountExpense)
#include "tst_tricountexpense.moc"
