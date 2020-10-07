#include "testcategory.h"

const QUuid ide = QUuid::createUuid();
const bool both = true;
const Account::TypeEnum type = Account::Income;
const QString name = "name1";

void TestCategory::initTestCase()
{
    model2.setId(ide);
    model2.setType(type);
    model2.setBoth(both);
}

void TestCategory::cleanupTestCase() {}
void TestCategory::testSetId()
{
    model.setId(ide);
    QVERIFY(true);
}

void TestCategory::testSetName()
{
    model.setName(name);
    QVERIFY(true);
}

void TestCategory::testSetType()
{
    model.setType(type);
    QVERIFY(true);
}

void TestCategory::testSetBoth()
{
    model.setBoth(both);
    QVERIFY(true);
}

void TestCategory::testGetId()
{
    QCOMPARE(model.id(), ide);
}

void TestCategory::testGetName()
{
    QCOMPARE(model.name(), name);
}

void TestCategory::testGetType()
{
    QCOMPARE(model.type(), type);
}

void TestCategory::testGetBoth()
{
    QCOMPARE(model.both(), both);
}

void TestCategory::testEquality()
{
    QCOMPARE(model, model2);
}

void TestCategory::testCopyContructor()
{
    copy = QSharedPointer<Category>::create(model);
    QCOMPARE(model, *copy);
}

void TestCategory::testCopy()
{
    *copy = model2;
    QCOMPARE(model2, *copy);
}

void TestCategory::test_jsonCast()
{
    QJsonObject obj = model;
    QCOMPARE(obj.keys(), model.metadataList());
}

void TestCategory::test_CategoryCast()
{
    QJsonObject obj = model;
    QVERIFY(obj.keys() == model.metadataList());
    Category e(obj);
    QCOMPARE(model, e);
}
