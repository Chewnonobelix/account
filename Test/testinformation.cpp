#include "testinformation.h"

const int id = 1;
const int ide = 1;
const QString category = "test category";
const bool estimated = true;
const QString title = "test title";

TestInformation::TestInformation(QObject *parent) : QObject(parent)
{

}

void TestInformation::initTestCase()
{    model2.setId(id);
     model2.setIdEntry(ide);
     model2.setCategory(category);
     model2.setEstimated(estimated);
     model2.setTitle(title);
}

void TestInformation::cleanupTestCase()
{}

void TestInformation::testSetId()
{
    model.setId(id);
    QVERIFY(true);
}

void TestInformation::testSetIdEntry()
{
    model.setIdEntry(ide);
    QVERIFY(true);
}

void TestInformation::testSetTitle()
{
    model.setTitle(title);
    QVERIFY(true);
}

void TestInformation::testSetEstimated()
{
    model.setEstimated(estimated);
}

void TestInformation::testSetCategory()
{
    model.setCategory(category);
    QVERIFY(true);
}

void TestInformation::testGetId()
{
    QCOMPARE(model.id(), id);
}

void TestInformation::testGetIdEntry()
{
    QCOMPARE(model.idEntry(), ide);
}

void TestInformation::testGetTitle()
{
    QCOMPARE(model.title(), title);
}

void TestInformation::testGetEstimated()
{
    QCOMPARE(model.estimated(), estimated);
}

void TestInformation::testGetCategory()
{
    QCOMPARE(model.category(), category);
}

void TestInformation::testCopy()
{
    copy.reset(nullptr);
    copy = QSharedPointer<Information>::create();
    *copy = model;
    QCOMPARE(*copy, model);
}

void TestInformation::testCopyConstructor()
{
    copy = QSharedPointer<Information>::create(model);
    QCOMPARE(*copy, model);
}

void TestInformation::testEquality()
{

    QCOMPARE(model, model2);
}

void TestInformation::testInferior()
{
    model2.setTitle(title+"_1");
    QVERIFY(model < model2);
}
