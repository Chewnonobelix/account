#ifndef TESTINFORMATION_H
#define TESTINFORMATION_H

#include <QObject>
#include <QTest>

#include "Model/information.h"

class TestInformation : public QObject
{
    Q_OBJECT
private:
    Information model, model2;
    QSharedPointer<Information> copy = nullptr;

public:
    explicit TestInformation(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testSetId();
    void testSetIdEntry();
    void testSetTitle();
    void testSetEstimated();
    void testSetCategory();

    void testGetId();
    void testGetIdEntry();
    void testGetTitle();
    void testGetEstimated();
    void testGetCategory();

    void testEquality();
    void testInferior();

    void testCopyConstructor();
    void testCopy();

signals:

};

#endif // TESTINFORMATION_H
