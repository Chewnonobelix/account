#include "testdebt.h"

void TestDebt::initTestCase() {}
void TestDebt::cleanupTestCase() {}

void TestDebt::test_id()
{
    QVERIFY(model.id().isNull());
    model.setId(id);
    QCOMPARE(model.id(), id);
}

void TestDebt::test_initial()
{
    model.setInitial(init);
    QCOMPARE(model.initial(), init);
}

void TestDebt::test_nb()
{
    model.setNb(nb);
    QCOMPARE(model.nb(), nb);
}

void TestDebt::test_rate()
{
    model.setRate(rate);
    QCOMPARE(model.rate(), rate);
}

void TestDebt::test_scheduler()
{
    model.setScheduler(scheduler);
    QCOMPARE(model.scheduler(), scheduler);
}

void TestDebt::test_last()
{
    model.setLast(last);
    QCOMPARE(model.last(), last);
}

void TestDebt::test_generate()
{
    QVERIFY(model.generate());
}

void TestDebt::testEquality()
{
    QCOMPARE(model, model2);
}

void TestDebt::testInferior()
{
    model2.setDate(date.addDays(3));
    QVERIFY(model < model2);
}

void TestDebt::testCopyContructor()
{
    copy = QSharedPointer<Debt>::create(model);
    QCOMPARE(model, *copy);
}

void TestDebt::testCopy()
{
    *copy = model2;
    QCOMPARE(model2, *copy);
}
