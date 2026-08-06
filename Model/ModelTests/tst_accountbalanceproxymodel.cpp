#include "../include/Model/Models/accountbalanceproxymodel.h"
#include "../include/Model/enums.h"
#include "../include/Model/Business/transaction.h"
#include "../include/Model/Models/transactionlistmodel.h"

#include <QDateTime>
#include <QSignalSpy>
#include <QtTest>

// --- Test helpers -----------------------------------------------------------

static TransactionPtr makeTx(const QDate &date, OpenAccountEnums::Movement mv,
                             double amount) {
  QJsonObject o;
  o.insert(QStringLiteral("date"),
           QDateTime(date.startOfDay()).toString(Qt::ISODate));
  o.insert(QStringLiteral("movement"), int(mv));
  o.insert(QStringLiteral("value"), amount);
  return QSharedPointer<Transaction>::create(o);
}

static double timestampOf(const QDate &date) {
  return static_cast<double>(
      QDateTime(date.startOfDay()).toMSecsSinceEpoch());
}

// --- Test case --------------------------------------------------------------

class TestAccountBalanceProxyModel : public QObject {
  Q_OBJECT

private slots:
  void empty_model_has_no_rows_and_flat_range() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setSourceModel(&source);

    QCOMPARE(proxy.rowCount(), 0);
    QCOMPARE(proxy.count(), 0);
    QCOMPARE(proxy.columnCount(), int(AccountBalanceProxyModel::ColumnCount));
    QCOMPARE(proxy.minBalance(), 0.0);
    QCOMPARE(proxy.maxBalance(), 0.0);
    QVERIFY(!proxy.minDate().isValid());
    QVERIFY(!proxy.maxDate().isValid());
  }

  void empty_model_range_follows_initial_balance() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setInitialBalance(250.0);
    proxy.setSourceModel(&source);

    QCOMPARE(proxy.minBalance(), 250.0);
    QCOMPARE(proxy.maxBalance(), 250.0);
  }

  void single_credit_accumulates_on_initial_balance() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setInitialBalance(100.0);
    proxy.setSourceModel(&source);

    source.addTransaction(
        makeTx(QDate(2025, 1, 10), OpenAccountEnums::Movement::Credit, 40.0));

    QCOMPARE(proxy.rowCount(), 1);
    const QModelIndex balanceIndex =
        proxy.index(0, AccountBalanceProxyModel::BalanceColumn);
    QCOMPARE(balanceIndex.data().toDouble(), 140.0);
  }

  void debit_is_subtracted() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setSourceModel(&source);

    source.addTransaction(
        makeTx(QDate(2025, 1, 10), OpenAccountEnums::Movement::Debit, 40.0));

    QCOMPARE(proxy
                 .index(0, AccountBalanceProxyModel::BalanceColumn)
                 .data()
                 .toDouble(),
             -40.0);
  }

  void rows_are_sorted_by_date_and_balance_is_cumulative() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setSourceModel(&source);

    // Inserted out of chronological order.
    source.addTransaction(
        makeTx(QDate(2025, 2, 1), OpenAccountEnums::Movement::Credit, 10.0));
    source.addTransaction(
        makeTx(QDate(2025, 1, 5), OpenAccountEnums::Movement::Debit, 40.0));
    source.addTransaction(
        makeTx(QDate(2025, 1, 10), OpenAccountEnums::Movement::Credit, 100.0));

    QCOMPARE(proxy.rowCount(), 3);

    // Chronological order: -40, then +100 => 60, then +10 => 70.
    QCOMPARE(proxy.index(0, 0).data(AccountBalanceProxyModel::DateRole).toDate(),
             QDate(2025, 1, 5));
    QCOMPARE(proxy.index(0, AccountBalanceProxyModel::BalanceColumn)
                 .data()
                 .toDouble(),
             -40.0);
    QCOMPARE(proxy.index(1, AccountBalanceProxyModel::BalanceColumn)
                 .data()
                 .toDouble(),
             60.0);
    QCOMPARE(proxy.index(2, AccountBalanceProxyModel::BalanceColumn)
                 .data()
                 .toDouble(),
             70.0);
  }

  void timestamp_column_matches_date() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setSourceModel(&source);

    source.addTransaction(
        makeTx(QDate(2025, 3, 15), OpenAccountEnums::Movement::Credit, 5.0));

    const QModelIndex xIndex =
        proxy.index(0, AccountBalanceProxyModel::TimestampColumn);
    QCOMPARE(xIndex.data().toDouble(), timestampOf(QDate(2025, 3, 15)));
    QCOMPARE(xIndex.data(AccountBalanceProxyModel::TimestampRole).toDouble(),
             timestampOf(QDate(2025, 3, 15)));
  }

  void roles_expose_date_balance_and_timestamp() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setSourceModel(&source);

    source.addTransaction(
        makeTx(QDate(2025, 5, 2), OpenAccountEnums::Movement::Credit, 30.0));

    const QModelIndex idx = proxy.index(0, 0);
    QCOMPARE(idx.data(AccountBalanceProxyModel::DateRole).toDate(),
             QDate(2025, 5, 2));
    QCOMPARE(idx.data(AccountBalanceProxyModel::BalanceRole).toDouble(), 30.0);
    QCOMPARE(idx.data(AccountBalanceProxyModel::TimestampRole).toDouble(),
             timestampOf(QDate(2025, 5, 2)));

    const QHash<int, QByteArray> names = proxy.roleNames();
    QCOMPARE(names.value(AccountBalanceProxyModel::DateRole), QByteArray("date"));
    QCOMPARE(names.value(AccountBalanceProxyModel::BalanceRole),
             QByteArray("balance"));
    QCOMPARE(names.value(AccountBalanceProxyModel::TimestampRole),
             QByteArray("timestamp"));
  }

  void range_tracks_min_max_and_dates() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setSourceModel(&source);

    source.addTransaction(
        makeTx(QDate(2025, 1, 1), OpenAccountEnums::Movement::Credit, 100.0));
    source.addTransaction(
        makeTx(QDate(2025, 1, 2), OpenAccountEnums::Movement::Debit, 250.0));
    source.addTransaction(
        makeTx(QDate(2025, 1, 3), OpenAccountEnums::Movement::Credit, 50.0));

    // Balances: 100, -150, -100 => min -150, max 100.
    QCOMPARE(proxy.minBalance(), -150.0);
    QCOMPARE(proxy.maxBalance(), 100.0);
    QCOMPARE(proxy.minDate(), QDateTime(QDate(2025, 1, 1).startOfDay()));
    QCOMPARE(proxy.maxDate(), QDateTime(QDate(2025, 1, 3).startOfDay()));
  }

  void initial_balance_change_rebuilds_series() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setSourceModel(&source);
    source.addTransaction(
        makeTx(QDate(2025, 1, 10), OpenAccountEnums::Movement::Credit, 40.0));

    QCOMPARE(proxy.index(0, AccountBalanceProxyModel::BalanceColumn)
                 .data()
                 .toDouble(),
             40.0);

    QSignalSpy rangeSpy(&proxy, &AccountBalanceProxyModel::rangeChanged);
    proxy.setInitialBalance(1000.0);

    QVERIFY(rangeSpy.count() >= 1);
    QCOMPARE(proxy.index(0, AccountBalanceProxyModel::BalanceColumn)
                 .data()
                 .toDouble(),
             1040.0);
  }

  void reacts_to_source_insertions() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setSourceModel(&source);

    QSignalSpy countSpy(&proxy, &AccountBalanceProxyModel::countChanged);

    source.addTransaction(
        makeTx(QDate(2025, 1, 1), OpenAccountEnums::Movement::Credit, 20.0));
    QCOMPARE(proxy.rowCount(), 1);

    source.addTransaction(
        makeTx(QDate(2025, 1, 2), OpenAccountEnums::Movement::Credit, 5.0));
    QCOMPARE(proxy.rowCount(), 2);
    QCOMPARE(proxy.index(1, AccountBalanceProxyModel::BalanceColumn)
                 .data()
                 .toDouble(),
             25.0);

    QVERIFY(countSpy.count() >= 2);
  }

  void reacts_to_source_removal() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setSourceModel(&source);

    auto tx =
        makeTx(QDate(2025, 1, 1), OpenAccountEnums::Movement::Credit, 20.0);
    source.addTransaction(tx);
    source.addTransaction(
        makeTx(QDate(2025, 1, 2), OpenAccountEnums::Movement::Credit, 5.0));
    QCOMPARE(proxy.rowCount(), 2);

    source.removeTransaction(tx->id());
    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.index(0, AccountBalanceProxyModel::BalanceColumn)
                 .data()
                 .toDouble(),
             5.0);
  }

  void map_to_and_from_source_round_trips() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setSourceModel(&source);

    // Source row 0 is dated later than source row 1, so proxy reorders them.
    source.addTransaction(
        makeTx(QDate(2025, 2, 1), OpenAccountEnums::Movement::Credit, 10.0));
    source.addTransaction(
        makeTx(QDate(2025, 1, 1), OpenAccountEnums::Movement::Credit, 10.0));

    // Proxy row 0 is the earlier transaction => source row 1.
    const QModelIndex sourceIndex = proxy.mapToSource(proxy.index(0, 0));
    QCOMPARE(sourceIndex.row(), 1);

    const QModelIndex backToProxy = proxy.mapFromSource(sourceIndex);
    QCOMPARE(backToProxy.row(), 0);
  }

  void monthly_frequency_keeps_end_of_month_balance() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setSourceModel(&source);

    source.addTransaction(
        makeTx(QDate(2025, 1, 5), OpenAccountEnums::Movement::Credit, 100.0));
    source.addTransaction(
        makeTx(QDate(2025, 1, 20), OpenAccountEnums::Movement::Debit, 30.0));
    source.addTransaction(
        makeTx(QDate(2025, 2, 10), OpenAccountEnums::Movement::Credit, 50.0));

    proxy.setPointFrequency(AccountBalanceProxyModel::Monthly);

    // One point per month; January keeps end-of-month balance (100 - 30 = 70),
    // February accumulates further (70 + 50 = 120).
    QCOMPARE(proxy.rowCount(), 2);
    QCOMPARE(proxy.index(0, 0).data(AccountBalanceProxyModel::DateRole).toDate(),
             QDate(2025, 1, 20));
    QCOMPARE(proxy.index(0, AccountBalanceProxyModel::BalanceColumn)
                 .data()
                 .toDouble(),
             70.0);
    QCOMPARE(proxy.index(1, AccountBalanceProxyModel::BalanceColumn)
                 .data()
                 .toDouble(),
             120.0);
  }

  void yearly_frequency_collapses_to_one_point_per_year() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setSourceModel(&source);

    source.addTransaction(
        makeTx(QDate(2023, 3, 1), OpenAccountEnums::Movement::Credit, 100.0));
    source.addTransaction(
        makeTx(QDate(2023, 11, 1), OpenAccountEnums::Movement::Credit, 100.0));
    source.addTransaction(
        makeTx(QDate(2024, 6, 1), OpenAccountEnums::Movement::Debit, 50.0));

    proxy.setPointFrequency(AccountBalanceProxyModel::Yearly);

    QCOMPARE(proxy.rowCount(), 2);
    QCOMPARE(proxy.index(0, AccountBalanceProxyModel::BalanceColumn)
                 .data()
                 .toDouble(),
             200.0);
    QCOMPARE(proxy.index(1, AccountBalanceProxyModel::BalanceColumn)
                 .data()
                 .toDouble(),
             150.0);
  }

  void window_restricts_span_but_keeps_accumulated_balance() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setSourceModel(&source);

    // Anchor (latest) is 2025-06-15. A one-year window keeps everything from
    // 2024-06-15 onward, but the balance still includes the older transaction.
    source.addTransaction(
        makeTx(QDate(2023, 1, 1), OpenAccountEnums::Movement::Credit, 1000.0));
    source.addTransaction(
        makeTx(QDate(2025, 1, 10), OpenAccountEnums::Movement::Credit, 100.0));
    source.addTransaction(
        makeTx(QDate(2025, 6, 15), OpenAccountEnums::Movement::Debit, 40.0));

    proxy.setReferenceDate(QDate(2025, 6, 15));
    proxy.setWindow(AccountBalanceProxyModel::Year);

    // The 2023 point is outside the window and dropped.
    QCOMPARE(proxy.rowCount(), 2);
    QCOMPARE(proxy.index(0, 0).data(AccountBalanceProxyModel::DateRole).toDate(),
             QDate(2025, 1, 10));
    // First visible balance already carries the 1000 from 2023.
    QCOMPARE(proxy.index(0, AccountBalanceProxyModel::BalanceColumn)
                 .data()
                 .toDouble(),
             1100.0);
    QCOMPARE(proxy.index(1, AccountBalanceProxyModel::BalanceColumn)
                 .data()
                 .toDouble(),
             1060.0);
  }

  void window_and_frequency_combine() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setSourceModel(&source);

    source.addTransaction(
        makeTx(QDate(2023, 1, 1), OpenAccountEnums::Movement::Credit, 500.0));
    source.addTransaction(
        makeTx(QDate(2025, 4, 3), OpenAccountEnums::Movement::Credit, 10.0));
    source.addTransaction(
        makeTx(QDate(2025, 4, 20), OpenAccountEnums::Movement::Credit, 10.0));
    source.addTransaction(
        makeTx(QDate(2025, 5, 2), OpenAccountEnums::Movement::Debit, 5.0));

    proxy.setReferenceDate(QDate(2025, 5, 2));
    proxy.setWindow(AccountBalanceProxyModel::Month);
    proxy.setPointFrequency(AccountBalanceProxyModel::Monthly);

    // Anchor 2025-05-02, one-month window keeps from 2025-04-02: April + May.
    QCOMPARE(proxy.rowCount(), 2);
    // April end balance: 500 + 10 + 10 = 520.
    QCOMPARE(proxy.index(0, AccountBalanceProxyModel::BalanceColumn)
                 .data()
                 .toDouble(),
             520.0);
    // May: 520 - 5 = 515.
    QCOMPARE(proxy.index(1, AccountBalanceProxyModel::BalanceColumn)
                 .data()
                 .toDouble(),
             515.0);
  }

  void window_anchors_on_today_by_default() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setSourceModel(&source);

    const QDate today = QDate::currentDate();
    source.addTransaction(
        makeTx(today.addYears(-2), OpenAccountEnums::Movement::Credit, 1000.0));
    source.addTransaction(
        makeTx(today.addDays(-3), OpenAccountEnums::Movement::Credit, 20.0));

    // No referenceDate set => anchored on today; a one-year window drops the
    // two-year-old transaction but its amount is still accumulated.
    proxy.setWindow(AccountBalanceProxyModel::Year);

    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.index(0, 0).data(AccountBalanceProxyModel::DateRole).toDate(),
             today.addDays(-3));
    QCOMPARE(proxy.index(0, AccountBalanceProxyModel::BalanceColumn)
                 .data()
                 .toDouble(),
             1020.0);
  }

  void reference_date_change_rebuilds_and_emits() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setSourceModel(&source);
    proxy.setWindow(AccountBalanceProxyModel::Month);

    source.addTransaction(
        makeTx(QDate(2025, 1, 15), OpenAccountEnums::Movement::Credit, 100.0));

    QSignalSpy refSpy(&proxy,
                      &AccountBalanceProxyModel::referenceDateChanged);

    // Anchored in the same month as the transaction => visible.
    proxy.setReferenceDate(QDate(2025, 1, 31));
    QCOMPARE(refSpy.count(), 1);
    QCOMPARE(proxy.rowCount(), 1);

    // Anchored months later => the January point falls out of the window.
    proxy.setReferenceDate(QDate(2025, 6, 1));
    QCOMPARE(proxy.rowCount(), 0);
  }

  void changing_granularity_emits_signals() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setSourceModel(&source);

    QSignalSpy windowSpy(&proxy, &AccountBalanceProxyModel::windowChanged);
    QSignalSpy freqSpy(&proxy,
                       &AccountBalanceProxyModel::pointFrequencyChanged);

    proxy.setWindow(AccountBalanceProxyModel::Decade);
    proxy.setPointFrequency(AccountBalanceProxyModel::Yearly);

    QCOMPARE(windowSpy.count(), 1);
    QCOMPARE(freqSpy.count(), 1);
    QCOMPARE(proxy.window(), AccountBalanceProxyModel::Decade);
    QCOMPARE(proxy.pointFrequency(), AccountBalanceProxyModel::Yearly);

    // Setting the same value again is a no-op.
    proxy.setWindow(AccountBalanceProxyModel::Decade);
    QCOMPARE(windowSpy.count(), 1);
  }

  void get_returns_row_as_map() {
    AccountBalanceProxyModel proxy;
    TransactionListModel source;
    proxy.setInitialBalance(10.0);
    proxy.setSourceModel(&source);
    source.addTransaction(
        makeTx(QDate(2025, 6, 1), OpenAccountEnums::Movement::Credit, 5.0));

    const QVariantMap map = proxy.get(0);
    QCOMPARE(map.value("date").toDate(), QDate(2025, 6, 1));
    QCOMPARE(map.value("balance").toDouble(), 15.0);
    QCOMPARE(map.value("timestamp").toDouble(), timestampOf(QDate(2025, 6, 1)));

    QVERIFY(proxy.get(-1).isEmpty());
    QVERIFY(proxy.get(99).isEmpty());
  }
};

QTEST_MAIN(TestAccountBalanceProxyModel)
#include "tst_accountbalanceproxymodel.moc"
