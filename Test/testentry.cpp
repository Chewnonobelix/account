#include "testentry.h"

const QUuid ide = QUuid::createUuid();
const QString account = "Test account";
const QString type = "type ytewsyt";
const double value = 2.12;
const bool blacked = true;
const QDate date = QDate::currentDate();

void TestEntry::initTestCase()
{
    model2.setId(ide);
    model2.setValue(value);
    model2.setDate(date);
    model2.setType(type);
    model2.setBlocked(blacked);
    model2.setAccount(account);
}

void TestEntry::cleanupTestCase()
{}

void TestEntry::testSetId()
{
    model.setId(ide);
    QVERIFY(true);
}

void TestEntry::testSetAccount()
{
    model.setAccount(account);
    QVERIFY(true);

}

void TestEntry::testSetValue()
{
    model.setValue(value);
    QVERIFY(true);
}

void TestEntry::testSetDate()
{
    model.setDate(date);
    QVERIFY(true);
}

void TestEntry::testSetType()
{
    model.setType(type);
    QVERIFY(true);
}

void TestEntry::testSetInfo()
{
    QSKIP("Maybe to suppress");
}

void TestEntry::testSetBlocked()
{
    model.setBlocked(blacked);
    QVERIFY(true);
}

void TestEntry::testGetId()
{
    QCOMPARE(model.id(), ide);
}

void TestEntry::testGetAccount()
{
    QCOMPARE(model.account(), account);
}

void TestEntry::testGetValue()
{
    QCOMPARE(model.value(), value);
}

void TestEntry::testGetDate()
{
    QCOMPARE(model.date(), date);
}

void TestEntry::testGetType()
{
    QCOMPARE(model.type(), type);
}

void TestEntry::testGetInfo()
{
    QSKIP("Maybe to supress");
}

void TestEntry::testGetBlocked()
{
    QVERIFY(model.isBlocked());
}

void TestEntry::testEquality()
{
    QCOMPARE(model, model2);
}

void TestEntry::testInferior()
{
    model2.setDate(date.addDays(3));
    QVERIFY(model < model2);
}

void TestEntry::testCopyContructor()
{
    copy = QSharedPointer<Entry>::create(model);
    QCOMPARE(model, *copy);
}

void TestEntry::testCopy()
{
    *copy = model2;
    QCOMPARE(model2, *copy);
}

void TestEntry::test_jsonCast()
{
    QJsonObject obj = model;
    QCOMPARE(obj.keys(), model.metadataList());
}

void TestEntry::test_EntryCast()
{
    QJsonObject obj = model;
    QVERIFY(obj.keys() == model.metadataList());
    Entry e(obj);
    QCOMPARE(model, e);
}
