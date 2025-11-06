#include <QtTest>
#include <QSignalSpy>
#include "Model/profile.h"

/*
 * Unit tests for Profile
 * Tests setters/getters, JSON round-trip, copy, validity, and signals.
 */

class TestProfile : public QObject
{
    Q_OBJECT

private slots:
    void testSetGet();
    void testJsonRoundTrip();
    void testValidity();
    void testSignal_firstNameChanged();
    void testSignal_lastNameChanged();
    void testSignal_accountsChanged();
    void testSignal_profileChangedCounts();
};

void TestProfile::testSetGet()
{
    Profile p;
    QUuid id = QUuid::createUuid();
    QString first = "Alice";
    QString last = "Dupont";
    QVector<QUuid> accounts = { QUuid::createUuid(), QUuid::createUuid() };

    p.setId(id);
    p.setFirstName(first);
    p.setLastName(last);
    p.setAccounts(accounts);

    QCOMPARE(p.id(), id);
    QCOMPARE(p.firstName(), first);
    QCOMPARE(p.lastName(), last);
    QCOMPARE(p.accounts(), accounts);
}

void TestProfile::testJsonRoundTrip()
{
    Profile p;
    p.setId(QUuid::createUuid());
    p.setFirstName("Jean");
    p.setLastName("Martin");
    QVector<QUuid> accounts = { QUuid::createUuid() };
    p.setAccounts(accounts);

    QJsonObject obj = p.toJson();
    Profile p2(obj);

    QCOMPARE(p2.id(), p.id());
    QCOMPARE(p2.firstName(), p.firstName());
    QCOMPARE(p2.lastName(), p.lastName());
    QCOMPARE(p2.accounts(), p.accounts());
}

void TestProfile::testValidity()
{
    Profile p;
    QVERIFY(!p.isValid());

    p.setId(QUuid::createUuid());
    QVERIFY(!p.isValid());

    p.setLastName("ValidLast");
    QVERIFY(p.isValid());
}

// --- Signal tests ---
void TestProfile::testSignal_firstNameChanged()
{
    Profile p;

    QSignalSpy spy(&p, SIGNAL(firstNameChanged()));
    p.setFirstName("Bob");
    QCOMPARE(spy.count(), 1);
}

void TestProfile::testSignal_lastNameChanged()
{
    Profile p;
    QSignalSpy spy(&p, SIGNAL(lastNameChanged()));
    p.setLastName("Marley");
    QCOMPARE(spy.count(), 1);
}

void TestProfile::testSignal_accountsChanged()
{
    Profile p;
    QSignalSpy spy(&p, SIGNAL(accountsChanged()));
    QVector<QUuid> acc = { QUuid::createUuid(), QUuid::createUuid() };
    p.setAccounts(acc);
    QCOMPARE(spy.count(), 1);
}

void TestProfile::testSignal_profileChangedCounts()
{
    Profile p;
    QSignalSpy spy(&p, SIGNAL(profileChanged()));

    p.setFirstName("A");
    p.setLastName("B");
    QVector<QUuid> acc = { QUuid::createUuid() };
    p.setAccounts(acc);

    QVERIFY(spy.count() >= 3);
}

#include "tst_Profile.moc"
