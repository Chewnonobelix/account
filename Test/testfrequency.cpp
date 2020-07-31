#include "testfrequency.h"

const QUuid id = QUuid::createUuid();
const Account::FrequencyEnum day = Account::FrequencyEnum::Day;
const Account::FrequencyEnum week = Account::FrequencyEnum::Week;
const Account::FrequencyEnum month = Account::FrequencyEnum::Month;
const Account::FrequencyEnum quarter = Account::FrequencyEnum::Quarter;
const Account::FrequencyEnum year = Account::FrequencyEnum::Year;
const QDate begin = QDate::currentDate();
const QDate end = QDate::currentDate().addYears(10);
const int nbgroup = 2;

void TestFrequency::initTestCase()
{
    filler.model = new QMap<QUuid, Frequency>;
}

void TestFrequency::cleanupTestCase()
{
    delete filler.model;
}

void TestFrequency::testSetId()
{
    model1.setId(id);
    QVERIFY(true);
}

void TestFrequency::testSetFreq()
{
    model1.setFreq(day);
     QVERIFY(true);
}
void TestFrequency::testSetBegin()
{
    model1.setBegin(begin);
    QVERIFY(true);
}

void TestFrequency::testSetEnd()
{
    model1.setEnd(end);
    QVERIFY(true);
}
void TestFrequency::testSetReference()
{
    model1.setReferenceEntry(reference);
    QVERIFY(true);
}

void TestFrequency::testSetGroup()
{
    model1.setNbGroup(nbgroup);
    QVERIFY(true);
}

void TestFrequency::testGetId()
{
    QCOMPARE(model1.id(), id);
}

void TestFrequency::testGetFreq()
{
    QCOMPARE(model1.freq(), day);
}

void TestFrequency::testGetBegin()
{
    QCOMPARE(model1.begin(), begin);
}

void TestFrequency::testGetEnd()
{
    QCOMPARE(model1.end(), end);
}

void TestFrequency::testGetReference()
{
    QCOMPARE(model1.referenceEntry(), reference);
}

void TestFrequency::testGetGroup()
{
    QCOMPARE(model1.nbGroup(), nbgroup);
}

void TestFrequency::testClone()
{
    auto clone = model1.clone(model1.referenceEntry());
    QCOMPARE(clone.metaData<QUuid>("frequency"), model1.id());
}

void TestFrequency::testFill()
{
    (*filler.model)[model1.id()] = model1;

    QDate it = begin;
    while(it <= end)
    {
        filler.entries<<model1.clone(model1.referenceEntry());
        filler.entries.last().setDate(it);
        it = it.addDays(Account::nbDay(it, model1.freq()));
    }

    QVERIFY(filler.entries.count() == begin.daysTo(end) + 1);
    QVERIFY(filler.model->count() == 1);
    filler.start();

    QVERIFY(filler.wait());
    model1 = filler.model->first();
}

void TestFrequency::testCount()
{
    QCOMPARE(model1.count(), begin.daysTo(end) + 1);
}

void TestFrequency::testList()
{}

void TestFrequency::testListGroup()
{}
