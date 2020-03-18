#ifndef TESTENTRY_H
#define TESTENTRY_H

#include <QObject>
#include <QTest>
#include "Model/entry.h"

class TestEntry : public QObject
{
    Q_OBJECT
private:
    Entry model, model2;
    QSharedPointer<Entry> copy = nullptr;

public:
    explicit TestEntry(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testSetId();
    void testSetAccount();
    void testSetValue();
    void testSetDate();
    void testSetType();
    void testSetInfo();
    void testSetBlocked();

    void testGetId();
    void testGetAccount();
    void testGetValue();
    void testGetDate();
    void testGetType();
    void testGetInfo();
    void testGetBlocked();

    void testEquality();
    void testInferior();

    void testCopyContructor();
    void testCopy();
signals:

};

#endif // TESTENTRY_H
