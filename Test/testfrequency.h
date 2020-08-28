#ifndef TESTFREQUENCY_H
#define TESTFREQUENCY_H

#include "Controller/features/filler.h"
#include "Model/frequency.h"
#include <QTest>

class TestFrequency: public QObject
{
    Q_OBJECT
public:
    TestFrequency() = default;
    Frequency model1, model2;
    QSharedPointer<Frequency> copy;
    Filler<QUuid, Frequency> filler;
    Entry reference;

private slots:
    void initTestCase();

    void testSetId();
    void testSetFreq();
    void testSetBegin();
    void testSetEnd();
    void testSetReference();
    void testSetGroup();

    void testGetId();
    void testGetFreq();
    void testGetBegin();
    void testGetEnd();
    void testGetReference();
    void testGetGroup();

    void testClone();

    void testFill();

    void testCount();

    void testList();
    void testListGroup();

    void cleanupTestCase();
signals:

};

#endif // TESTFREQUENCY_H
