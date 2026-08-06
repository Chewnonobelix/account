#include "../include/Model/Business/budget.h"

#include <QSignalSpy>
#include <QtTest/QtTest>

namespace {
TransactionPtr makeTransaction(QUuid category, QDate date, double value) {
    TransactionPtr t = TransactionPtr::create();
    t->setCategory(category);
    t->setDate(date);
    t->setValue(value);
    return t;
}
} // namespace

class TestBudget : public QObject {
  Q_OBJECT

private slots:
  void defaults_are_sane() {
    Budget budget;

    QVERIFY(!budget.id().isNull());
    QCOMPARE(budget.name(), QString());
    QVERIFY(budget.categoryId().isNull());
    QCOMPARE(budget.threshold(), 0.0);
    QCOMPARE(budget.startDate(), QDate());
    QCOMPARE(budget.endDate(), QDate());
    QCOMPARE(budget.recurrence(), OpenAccountEnums::Frequency::Once);
    QVERIFY(!budget.isRepeatable());
    QCOMPARE(budget.transactions().size(), 0);
    QCOMPARE(budget.usedAmount(), 0.0);
    QCOMPARE(budget.remaining(), 0.0);
    QCOMPARE(budget.usageRatio(), 0.0);
    QVERIFY(!budget.isOverBudget());
  }

  void setters_apply_guards() {
    Budget budget;

    budget.setThreshold(-50.0);
    QCOMPARE(budget.threshold(), 0.0);

    budget.setThreshold(200.0);
    QCOMPARE(budget.threshold(), 200.0);

    budget.setRecurrence(OpenAccountEnums::Frequency::Custom);
    QCOMPARE(budget.recurrence(), OpenAccountEnums::Frequency::Once);

    budget.setRecurrence(OpenAccountEnums::Frequency::Monthly);
    QCOMPARE(budget.recurrence(), OpenAccountEnums::Frequency::Monthly);
    QVERIFY(budget.isRepeatable());
  }

  void isWithinPeriod_requiresValidBoundsAndRange() {
    Budget budget;

    QVERIFY(!budget.isWithinPeriod(QDate(2026, 1, 15)));

    budget.setStartDate(QDate(2026, 1, 1));
    budget.setEndDate(QDate(2026, 1, 31));

    QVERIFY(budget.isWithinPeriod(QDate(2026, 1, 1)));
    QVERIFY(budget.isWithinPeriod(QDate(2026, 1, 31)));
    QVERIFY(budget.isWithinPeriod(QDate(2026, 1, 15)));
    QVERIFY(!budget.isWithinPeriod(QDate(2025, 12, 31)));
    QVERIFY(!budget.isWithinPeriod(QDate(2026, 2, 1)));
  }

  void matches_checksCategoryAndPeriod() {
    Budget budget;
    const QUuid categoryId = QUuid::createUuid();
    budget.setCategoryId(categoryId);
    budget.setStartDate(QDate(2026, 1, 1));
    budget.setEndDate(QDate(2026, 1, 31));

    QVERIFY(budget.matches(makeTransaction(categoryId, QDate(2026, 1, 15), 10.0)));
    QVERIFY(!budget.matches(makeTransaction(QUuid::createUuid(), QDate(2026, 1, 15), 10.0)));
    QVERIFY(!budget.matches(makeTransaction(categoryId, QDate(2026, 2, 1), 10.0)));
    QVERIFY(!budget.matches(TransactionPtr()));
  }

  void addTransaction_ignoresNonMatchingAndDuplicates() {
    Budget budget;
    const QUuid categoryId = QUuid::createUuid();
    budget.setCategoryId(categoryId);
    budget.setStartDate(QDate(2026, 1, 1));
    budget.setEndDate(QDate(2026, 1, 31));

    TransactionPtr outsideCategory = makeTransaction(QUuid::createUuid(), QDate(2026, 1, 10), 10.0);
    QVERIFY(!budget.addTransaction(outsideCategory));
    QCOMPARE(budget.transactions().size(), 0);

    TransactionPtr matching = makeTransaction(categoryId, QDate(2026, 1, 10), 25.0);
    QVERIFY(budget.addTransaction(matching));
    QCOMPARE(budget.transactions().size(), 1);

    QVERIFY(!budget.addTransaction(matching));
    QCOMPARE(budget.transactions().size(), 1);
  }

  void usage_reflectsAttachedTransactions() {
    Budget budget;
    const QUuid categoryId = QUuid::createUuid();
    budget.setCategoryId(categoryId);
    budget.setStartDate(QDate(2026, 1, 1));
    budget.setEndDate(QDate(2026, 1, 31));
    budget.setThreshold(100.0);

    budget.addTransaction(makeTransaction(categoryId, QDate(2026, 1, 5), -30.0));
    budget.addTransaction(makeTransaction(categoryId, QDate(2026, 1, 20), 20.0));

    QCOMPARE(budget.usedAmount(), 50.0);
    QCOMPARE(budget.remaining(), 50.0);
    QCOMPARE(budget.usageRatio(), 50.0);
    QVERIFY(!budget.isOverBudget());

    budget.addTransaction(makeTransaction(categoryId, QDate(2026, 1, 25), 60.0));

    QCOMPARE(budget.usedAmount(), 110.0);
    QCOMPARE(budget.remaining(), -10.0);
    QCOMPARE(budget.usageRatio(), 110.0);
    QVERIFY(budget.isOverBudget());
  }

  void usageRatio_isZeroWhenThresholdIsNotPositive() {
    Budget budget;
    const QUuid categoryId = QUuid::createUuid();
    budget.setCategoryId(categoryId);
    budget.setStartDate(QDate(2026, 1, 1));
    budget.setEndDate(QDate(2026, 1, 31));

    budget.addTransaction(makeTransaction(categoryId, QDate(2026, 1, 5), 30.0));

    QCOMPARE(budget.threshold(), 0.0);
    QCOMPARE(budget.usageRatio(), 0.0);
  }

  void removeTransaction_and_clearTransactions_updateUsage() {
    Budget budget;
    const QUuid categoryId = QUuid::createUuid();
    budget.setCategoryId(categoryId);
    budget.setStartDate(QDate(2026, 1, 1));
    budget.setEndDate(QDate(2026, 1, 31));
    budget.setThreshold(100.0);

    TransactionPtr t1 = makeTransaction(categoryId, QDate(2026, 1, 5), 30.0);
    TransactionPtr t2 = makeTransaction(categoryId, QDate(2026, 1, 6), 20.0);
    budget.addTransaction(t1);
    budget.addTransaction(t2);
    QCOMPARE(budget.usedAmount(), 50.0);

    budget.removeTransaction(t1->id());
    QCOMPARE(budget.transactions().size(), 1);
    QCOMPARE(budget.usedAmount(), 20.0);

    budget.clearTransactions();
    QCOMPARE(budget.transactions().size(), 0);
    QCOMPARE(budget.usedAmount(), 0.0);
  }

  void json_roundtrip_restores_values() {
    Budget budget;
    const QUuid id = QUuid::createUuid();
    const QUuid categoryId = QUuid::createUuid();

    budget.setId(id);
    budget.setName(QStringLiteral("Courses"));
    budget.setCategoryId(categoryId);
    budget.setThreshold(300.0);
    budget.setStartDate(QDate(2026, 1, 1));
    budget.setEndDate(QDate(2026, 1, 31));
    budget.setRecurrence(OpenAccountEnums::Frequency::Monthly);

    const Budget restored(budget.toJson());

    QCOMPARE(restored.id(), id);
    QCOMPARE(restored.name(), budget.name());
    QCOMPARE(restored.categoryId(), categoryId);
    QCOMPARE(restored.threshold(), budget.threshold());
    QCOMPARE(restored.startDate(), budget.startDate());
    QCOMPARE(restored.endDate(), budget.endDate());
    QCOMPARE(restored.recurrence(), budget.recurrence());
    QCOMPARE(restored.transactions().size(), 0);
  }

  void signals_fire_once_per_effective_change() {
    Budget budget;
    QSignalSpy changedSpy(&budget, &Budget::changed);
    QSignalSpy thresholdSpy(&budget, &Budget::thresholdChanged);
    QSignalSpy usageSpy(&budget, &Budget::usageChanged);

    budget.setThreshold(100.0);
    budget.setThreshold(100.0);
    QCOMPARE(thresholdSpy.count(), 1);
    QCOMPARE(usageSpy.count(), 1);

    const QUuid categoryId = QUuid::createUuid();
    budget.setCategoryId(categoryId);
    budget.setStartDate(QDate(2026, 1, 1));
    budget.setEndDate(QDate(2026, 1, 31));
    usageSpy.clear();

    TransactionPtr t = makeTransaction(categoryId, QDate(2026, 1, 5), 10.0);
    budget.addTransaction(t);
    QCOMPARE(usageSpy.count(), 1);

    budget.removeTransaction(t->id());
    QCOMPARE(usageSpy.count(), 2);

    QCOMPARE(changedSpy.count(), 1 /*threshold*/ + 3 /*categoryId+startDate+endDate*/ + 2 /*add+remove*/);
  }
};

QTEST_MAIN(TestBudget)
#include "tst_budget.moc"
