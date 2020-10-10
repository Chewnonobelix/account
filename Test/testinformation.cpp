#include "testinformation.h"

Category category;
const bool estimated = true;
const QString title = "test title";

TestInformation::TestInformation(QObject *parent) : QObject(parent)
{

}

void TestInformation::initTestCase()
{
    category.setName("test category");
    category.setId(QUuid::createUuid());
    model2.setCategory(category);
    model2.setEstimated(estimated);
    model2.setTitle(title);
}

void TestInformation::cleanupTestCase()
{}

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
    copy = QSharedPointer<Entry>::create();
    *copy = model;
    QCOMPARE(*copy, model);
}

void TestInformation::testCopyConstructor()
{
    copy = QSharedPointer<Entry>::create(model);
    QCOMPARE(*copy, model);
}

void TestInformation::testEquality()
{

    QCOMPARE(model, model2);
}
