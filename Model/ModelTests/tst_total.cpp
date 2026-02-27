#include "../include/Model/enums.h"
#include "../include/Model/total.h"
#include "../include/Model/transaction.h"
#include <QMetaEnum>
#include <QtTest>

// --- Test helpers -----------------------------------------------------------

static QSharedPointer<Transaction>
makeTx(const QDate &date, OpenAccountEnums::Movement mv, double amount) {
  QJsonObject o;
  // If Transaction expects ISO string for date:
  o.insert(QStringLiteral("date"),
           QDateTime(date.startOfDay()).toString(Qt::ISODate));
  // Write enum as string key (Q_ENUM).
  o.insert(QStringLiteral("movement"), int(mv));
  o.insert(QStringLiteral("value"), amount);
  return QSharedPointer<Transaction>::create(o);
}

// --- Test case --------------------------------------------------------------
class tst_Total : public QObject {
  Q_OBJECT
private slots:
  void empty_total_is_zero() {
    Total t;
    QCOMPARE(t.evaluate(), 0.0);
    QVERIFY(!t.from().isValid());
    QVERIFY(!t.to().isValid());
  }

  void add_single_transaction_sets_bounds_and_sum_credit() {
    Total tot;
    auto tx =
        makeTx(QDate(2025, 1, 10), OpenAccountEnums::Movement::Credit, 100.0);
    tot.addTransaction(tx);

    QCOMPARE(tot.from(), QDate(2025, 1, 10));
    QCOMPARE(tot.to(), QDate(2025, 1, 10));
    QCOMPARE(tot.evaluate(), 100.0);
  }

  void add_single_transaction_sets_bounds_and_sum_debit() {
    Total tot;
    auto tx =
        makeTx(QDate(2025, 1, 10), OpenAccountEnums::Movement::Debit, 40.0);
    tot.addTransaction(tx);

    QCOMPARE(tot.from(), QDate(2025, 1, 10));
    QCOMPARE(tot.to(), QDate(2025, 1, 10));
    QCOMPARE(tot.evaluate(), -40.0);
  }

  void add_multiple_updates_bounds_and_sum_mixed() {
    Total tot;
    tot.addTransaction(
        makeTx(QDate(2025, 1, 10), OpenAccountEnums::Movement::Credit, 100.0));
    tot.addTransaction(
        makeTx(QDate(2025, 1, 05), OpenAccountEnums::Movement::Debit, 40.0));
    tot.addTransaction(
        makeTx(QDate(2025, 2, 01), OpenAccountEnums::Movement::Credit, 10.0));

    QCOMPARE(tot.from(), QDate(2025, 1, 5));
    QCOMPARE(tot.to(), QDate(2025, 2, 1));
    // 100 - 40 + 10 = 70
    QCOMPARE(tot.evaluate(), 70.0);
  }

  void plus_total_total_concatenates_and_extends_bounds() {
    Total a;
    a.addTransaction(
        makeTx(QDate(2025, 3, 01), OpenAccountEnums::Movement::Credit, 50.0));
    a.addTransaction(
        makeTx(QDate(2025, 3, 05), OpenAccountEnums::Movement::Debit, 20.0));

    Total b;
    b.addTransaction(
        makeTx(QDate(2025, 2, 27), OpenAccountEnums::Movement::Credit, 5.0));
    b.addTransaction(
        makeTx(QDate(2025, 3, 10), OpenAccountEnums::Movement::Debit, 10.0));

    Total c = a + b;

    QCOMPARE(c.from(), QDate(2025, 2, 27));
    QCOMPARE(c.to(), QDate(2025, 3, 10));
    // (50 - 20) + (5 - 10) = 25
    QCOMPARE(c.evaluate(), 25.0);
  }

  void plus_total_transaction_appends_one() {
    Total a;
    a.addTransaction(
        makeTx(QDate(2025, 1, 01), OpenAccountEnums::Movement::Credit, 20.0));
    Total c =
        a + makeTx(QDate(2025, 1, 03), OpenAccountEnums::Movement::Debit, 5.0);

    QCOMPARE(c.from(), QDate(2025, 1, 1));
    QCOMPARE(c.to(), QDate(2025, 1, 3));
    QCOMPARE(c.evaluate(), 15.0);
  }
};

#include "tst_total.moc"
