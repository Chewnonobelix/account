#ifndef TESTCATEGORY_H
#define TESTCATEGORY_H

#include "Model/category.h"
#include <QTest>
#include <QtCore/QObject>
#include <QtCore/qglobal.h>

class TestCategory : public QObject
{
    Q_OBJECT
private:
    Category model, model2;
    QSharedPointer<Category> copy = nullptr;

public:
    explicit TestCategory() = default;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testSetId();
    void testSetName();
    void testSetType();
    void testSetBoth();
    void testGetId();
    void testGetName();
    void testGetType();
    void testGetBoth();

    void testEquality();

    void testCopyContructor();
    void testCopy();

    void test_jsonCast();
    void test_CategoryCast();
};

#endif // TESTCATEGORY_H
