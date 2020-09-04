#ifndef TESTDEBT_HPP
#define TESTDEBT_HPP

#include "Model/debt.h"
#include <QTest>
#include <QtCore/qglobal.h>

class TestDebt : public QObject
{
    Q_OBJECT
private:
    Debt model1, model2;
    QSharedPointer<Debt> copy = nullptr;

    const double rate = 0.3;
    const int nb = 12;
    const QUuid id = QUuid::createUuid();
    Entry init, last;
    Frequency scheduler;

public:
    TestDebt() = default;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_id();
    void test_initial();
    void test_nb();
    void test_rate();
    void test_scheduler();
    void test_last();
};

#endif // TESTDEBT_HPP
