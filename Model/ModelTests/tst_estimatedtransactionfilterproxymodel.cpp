#include "../include/Model/Business/transaction.h"
#include "../include/Model/Models/estimatedtransactionfilterproxymodel.h"
#include "../include/Model/Models/transactionlistmodel.h"

#include <QSignalSpy>
#include <QtTest/QtTest>

class TestEstimatedTransactionFilterProxyModel : public QObject {
  Q_OBJECT

private:
  static TransactionPtr makeTransaction(const QString &description,
                                        const QDate &date, bool estimated) {
    TransactionPtr transaction = TransactionPtr::create();
    transaction->setDescription(description);
    transaction->setDate(date);
    transaction->setEstimated(estimated);
    return transaction;
  }

private slots:
  void emptySource_showsNothing() {
    TransactionListModel sourceModel;
    EstimatedTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);

    QCOMPARE(proxy.rowCount(), 0);
  }

  void defaultReferenceDate_isToday() {
    EstimatedTransactionFilterProxyModel proxy;
    QCOMPARE(proxy.referenceDate(), QDate::currentDate());
  }

  void estimatedPastDate_isIncluded() {
    const QDate today = QDate::currentDate();
    const TransactionPtr kept =
        makeTransaction(QStringLiteral("Kept"), today.addDays(-5), true);

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(kept));

    EstimatedTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);

    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.at(0), static_cast<QObject *>(kept.data()));
  }

  void estimatedFutureOrTodayDate_isExcluded() {
    const QDate today = QDate::currentDate();
    const TransactionPtr future =
        makeTransaction(QStringLiteral("Future"), today.addDays(5), true);
    const TransactionPtr sameDay =
        makeTransaction(QStringLiteral("Today"), today, true);

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(future));
    QVERIFY(sourceModel.addTransaction(sameDay));

    EstimatedTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);

    QCOMPARE(proxy.rowCount(), 0);
  }

  void nonEstimatedPastDate_isExcluded() {
    const QDate today = QDate::currentDate();
    const TransactionPtr notEstimated =
        makeTransaction(QStringLiteral("Confirmed"), today.addDays(-5), false);

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(notEstimated));

    EstimatedTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);

    QCOMPARE(proxy.rowCount(), 0);
  }

  void settingReferenceDate_reappliesTheFilter() {
    const TransactionPtr transaction = makeTransaction(
        QStringLiteral("Mid"), QDate(2026, 1, 15), true);

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(transaction));

    EstimatedTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    proxy.setReferenceDate(QDate(2026, 1, 10)); // before the transaction
    QCOMPARE(proxy.rowCount(), 0);

    QSignalSpy referenceDateSpy(&proxy,
                                &EstimatedTransactionFilterProxyModel::referenceDateChanged);
    proxy.setReferenceDate(QDate(2026, 1, 20)); // after the transaction

    QCOMPARE(referenceDateSpy.count(), 1);
    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.at(0), static_cast<QObject *>(transaction.data()));
  }

  void settingSameReferenceDate_doesNotEmitChanged() {
    EstimatedTransactionFilterProxyModel proxy;
    proxy.setReferenceDate(QDate(2026, 1, 20));

    QSignalSpy spy(&proxy, &EstimatedTransactionFilterProxyModel::referenceDateChanged);
    proxy.setReferenceDate(QDate(2026, 1, 20));
    QCOMPARE(spy.count(), 0);
  }

  void estimatedFlagChange_reappliesTheFilter() {
    const QDate today = QDate::currentDate();
    const TransactionPtr transaction =
        makeTransaction(QStringLiteral("Pending"), today.addDays(-5), true);

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(transaction));

    EstimatedTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    QCOMPARE(proxy.rowCount(), 1);

    QSignalSpy countSpy(&proxy, &EstimatedTransactionFilterProxyModel::countChanged);
    transaction->setEstimated(false); // now confirmed: should drop out

    QCOMPARE(proxy.rowCount(), 0);
    QVERIFY(countSpy.count() >= 1);
  }

  void combinedFilters_areAllApplied() {
    const QDate today = QDate::currentDate();
    const TransactionPtr kept =
        makeTransaction(QStringLiteral("Kept"), today.addDays(-1), true);
    const TransactionPtr wrongEstimated =
        makeTransaction(QStringLiteral("WrongEstimated"), today.addDays(-1), false);
    const TransactionPtr wrongDate =
        makeTransaction(QStringLiteral("WrongDate"), today.addDays(1), true);

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(kept));
    QVERIFY(sourceModel.addTransaction(wrongEstimated));
    QVERIFY(sourceModel.addTransaction(wrongDate));

    EstimatedTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);

    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.at(0), static_cast<QObject *>(kept.data()));
  }
};

QTEST_MAIN(TestEstimatedTransactionFilterProxyModel)
#include "tst_estimatedtransactionfilterproxymodel.moc"
