#include "testdebt.h"

void TestDebt::initTestCase() {
    init.setValue(2000);
    init.setType("income");
    init.setId(QUuid::createUuid());
    init.setDate(QDate::currentDate());
}

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

//void TestDebt::test_scheduler()
//{
//    model.setScheduler(scheduler);
//    QCOMPARE(model.scheduler(), scheduler);
//}

void TestDebt::test_generate()
{
    QVERIFY(model.generate());

    Total t;
    auto list = model.entries();

    for (auto it : list) {
        t = t + it;
    }

    QCOMPARE(t.value(), init.value() + init.value() * rate);
}

void TestDebt::test_entries()
{
    QCOMPARE(model.entries().size(), nb);
}

void TestDebt::testEquality()
{
    QCOMPARE(model, model2);
}

void TestDebt::testInferior()
{
    model2.setRate(rate + 0.1);
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
