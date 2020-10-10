#ifndef TESTINFORMATION_H
#define TESTINFORMATION_H

#include <QObject>
#include <QTest>

#include "Model/entry.h"

class TestInformation : public QObject
{
    Q_OBJECT
private:
    Entry model, model2;
    QSharedPointer<Entry> copy = nullptr;

public:
    explicit TestInformation(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testSetTitle();
    void testSetEstimated();
    void testSetCategory();

    void testGetTitle();
    void testGetEstimated();
    void testGetCategory();

    void testEquality();

    void testCopyConstructor();
    void testCopy();

signals:

};

#endif // TESTINFORMATION_H
