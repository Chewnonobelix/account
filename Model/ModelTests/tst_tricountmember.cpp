#include "../include/Model/Business/tricountmember.h"
#include <QSignalSpy>
#include <QtTest>

/*
 * Unit tests for TricountMember
 * Tests setters/getters, guest detection, JSON round-trip, validity, signals.
 */

class TestTricountMember : public QObject {
    Q_OBJECT

private slots:
    void testSetGet();
    void testGuestVsProfileLinked();
    void testValidity();
    void testJsonRoundTrip();
    void testSignal_nameChanged();
    void testSignal_profileIdChanged();
    void testSignal_changedCounts();
};

void TestTricountMember::testSetGet() {
    TricountMember m;
    const QUuid id = QUuid::createUuid();
    const QUuid profile = QUuid::createUuid();
    const QUuid account = QUuid::createUuid();

    m.setId(id);
    m.setName("Alice");
    m.setProfileId(profile);
    m.setAccountId(account);

    QCOMPARE(m.id(), id);
    QCOMPARE(m.name(), QString("Alice"));
    QCOMPARE(m.profileId(), profile);
    QCOMPARE(m.accountId(), account);
}

void TestTricountMember::testGuestVsProfileLinked() {
    TricountMember guest;
    guest.setName("Bob");
    QVERIFY(guest.isGuest());

    TricountMember linked;
    linked.setName("Carol");
    linked.setProfileId(QUuid::createUuid());
    QVERIFY(!linked.isGuest());
}

void TestTricountMember::testValidity() {
    TricountMember m;
    QVERIFY(!m.isValid());

    m.setId(QUuid::createUuid());
    QVERIFY(!m.isValid());

    m.setName("Dan");
    QVERIFY(m.isValid());
}

void TestTricountMember::testJsonRoundTrip() {
    TricountMember m;
    m.setId(QUuid::createUuid());
    m.setName("Eve");
    m.setProfileId(QUuid::createUuid());
    m.setAccountId(QUuid::createUuid());

    const QJsonObject obj = m.toJson();
    TricountMember copy(obj);

    QCOMPARE(copy.id(), m.id());
    QCOMPARE(copy.name(), m.name());
    QCOMPARE(copy.profileId(), m.profileId());
    QCOMPARE(copy.accountId(), m.accountId());
    QCOMPARE(copy.isGuest(), m.isGuest());
}

void TestTricountMember::testSignal_nameChanged() {
    TricountMember m;
    QSignalSpy spy(&m, SIGNAL(nameChanged()));
    m.setName("Frank");
    QCOMPARE(spy.count(), 1);
    m.setName("Frank"); // no-op
    QCOMPARE(spy.count(), 1);
}

void TestTricountMember::testSignal_profileIdChanged() {
    TricountMember m;
    QSignalSpy spy(&m, SIGNAL(profileIdChanged()));
    m.setProfileId(QUuid::createUuid());
    QCOMPARE(spy.count(), 1);
}

void TestTricountMember::testSignal_changedCounts() {
    TricountMember m;
    QSignalSpy spy(&m, SIGNAL(changed()));
    m.setName("G");
    m.setProfileId(QUuid::createUuid());
    m.setAccountId(QUuid::createUuid());
    QVERIFY(spy.count() >= 3);
}

QTEST_MAIN(TestTricountMember)
#include "tst_tricountmember.moc"
