#include "../include/Model/Business/account.h"
#include "../include/Model/Models/accounttransactionfilterproxymodel.h"
#include "../include/Model/enums.h"
#include "../include/Model/Business/transaction.h"
#include "../include/Model/Models/transactionlistmodel.h"

#include <QSignalSpy>
#include <QtTest/QtTest>

class TestAccountTransactionFilterProxyModel : public QObject {
  Q_OBJECT

private:
  static TransactionPtr makeTransaction(
      const QString &description, const QUuid &accountId,
      const QDate &date = QDate(),
      OpenAccountEnums::Support support = OpenAccountEnums::Support::Other,
      const QUuid &category = QUuid()) {
    TransactionPtr transaction = TransactionPtr::create();
    transaction->setDescription(description);
    transaction->setValue(10.0);
    transaction->setAccountId(accountId);
    transaction->setDate(date);
    transaction->setSupport(support);
    transaction->setCategory(category);
    return transaction;
  }

private slots:
  void withoutAccount_showsEverything() {
    const QUuid firstAccountId = QUuid::createUuid();
    const QUuid secondAccountId = QUuid::createUuid();

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(
        makeTransaction(QStringLiteral("A"), firstAccountId)));
    QVERIFY(sourceModel.addTransaction(
        makeTransaction(QStringLiteral("B"), secondAccountId)));

    AccountTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);

    QCOMPARE(proxy.rowCount(), 2);
  }

  void setAccount_filtersTransactionsByAccountId() {
    Account account;
    const QUuid selectedAccountId = QUuid::createUuid();
    account.setId(selectedAccountId);

    const TransactionPtr selected =
        makeTransaction(QStringLiteral("Selected"), selectedAccountId);
    const TransactionPtr other =
        makeTransaction(QStringLiteral("Other"), QUuid::createUuid());

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(selected));
    QVERIFY(sourceModel.addTransaction(other));

    AccountTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    proxy.setAccount(&account);

    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.data(proxy.index(0, 0), TransactionListModel::DescriptionRole)
                 .toString(),
             QStringLiteral("Selected"));
    QCOMPARE(proxy.at(0), static_cast<QObject *>(selected.data()));
    QCOMPARE(proxy.get(0).value(QStringLiteral("accountId")).toUuid(),
             selectedAccountId);
  }

  void transactionAccountIdChanges_reapplyTheFilter() {
    Account account;
    const QUuid selectedAccountId = QUuid::createUuid();
    account.setId(selectedAccountId);

    const TransactionPtr first =
        makeTransaction(QStringLiteral("Included"), selectedAccountId);
    const TransactionPtr second =
        makeTransaction(QStringLiteral("Hidden"), QUuid::createUuid());

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(first));
    QVERIFY(sourceModel.addTransaction(second));

    AccountTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    proxy.setAccount(&account);

    QSignalSpy rowsRemovedSpy(&proxy, &QAbstractItemModel::rowsRemoved);
    QSignalSpy rowsInsertedSpy(&proxy, &QAbstractItemModel::rowsInserted);

    first->setAccountId(QUuid::createUuid());
    second->setAccountId(selectedAccountId);

    QVERIFY(rowsRemovedSpy.count() >= 1);
    QVERIFY(rowsInsertedSpy.count() >= 1);
    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.at(0), static_cast<QObject *>(second.data()));
  }

  void accountIdChanges_refreshTheVisibleTransactions() {
    Account account;
    const QUuid firstAccountId = QUuid::createUuid();
    const QUuid secondAccountId = QUuid::createUuid();
    account.setId(firstAccountId);

    const TransactionPtr first =
        makeTransaction(QStringLiteral("First"), firstAccountId);
    const TransactionPtr second =
        makeTransaction(QStringLiteral("Second"), secondAccountId);

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(first));
    QVERIFY(sourceModel.addTransaction(second));

    AccountTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    proxy.setAccount(&account);

    account.setId(secondAccountId);

    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.at(0), static_cast<QObject *>(second.data()));
  }

  void emptyFilters_showEverything() {
    const QUuid accountId = QUuid::createUuid();

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(makeTransaction(
        QStringLiteral("A"), accountId, QDate(2026, 1, 1),
        OpenAccountEnums::Support::Cash, QUuid::createUuid())));
    QVERIFY(sourceModel.addTransaction(makeTransaction(
        QStringLiteral("B"), accountId, QDate(2026, 2, 1),
        OpenAccountEnums::Support::Card, QUuid::createUuid())));

    AccountTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);

    QVERIFY(proxy.dateFilter().isEmpty());
    QVERIFY(!proxy.dateFrom().isValid());
    QVERIFY(!proxy.dateTo().isValid());
    QVERIFY(proxy.supportFilter().isEmpty());
    QVERIFY(proxy.categoryFilter().isEmpty());
    QVERIFY(proxy.descriptionFilter().isEmpty());
    QCOMPARE(proxy.rowCount(), 2);
  }

  void dateFilter_showsOnlyMatchingDates() {
    const QUuid accountId = QUuid::createUuid();
    const QDate keptDate(2026, 1, 15);
    const QDate otherDate(2026, 2, 20);

    const TransactionPtr kept =
        makeTransaction(QStringLiteral("Kept"), accountId, keptDate);
    const TransactionPtr other =
        makeTransaction(QStringLiteral("Other"), accountId, otherDate);

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(kept));
    QVERIFY(sourceModel.addTransaction(other));

    AccountTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);

    QSignalSpy filterChangedSpy(&proxy,
                                &AccountTransactionFilterProxyModel::dateFilterChanged);
    proxy.setDateFilter({keptDate});

    QCOMPARE(filterChangedSpy.count(), 1);
    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.at(0), static_cast<QObject *>(kept.data()));
  }

  void dateRange_showsOnlyDatesWithinBounds() {
    const QUuid accountId = QUuid::createUuid();

    const TransactionPtr before =
        makeTransaction(QStringLiteral("Before"), accountId, QDate(2026, 1, 9));
    const TransactionPtr kept =
        makeTransaction(QStringLiteral("Kept"), accountId, QDate(2026, 1, 15));
    const TransactionPtr after =
        makeTransaction(QStringLiteral("After"), accountId, QDate(2026, 1, 21));

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(before));
    QVERIFY(sourceModel.addTransaction(kept));
    QVERIFY(sourceModel.addTransaction(after));

    AccountTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);

    QSignalSpy fromSpy(&proxy, &AccountTransactionFilterProxyModel::dateFromChanged);
    QSignalSpy toSpy(&proxy, &AccountTransactionFilterProxyModel::dateToChanged);
    proxy.setDateFrom(QDate(2026, 1, 10));
    proxy.setDateTo(QDate(2026, 1, 20));

    QCOMPARE(fromSpy.count(), 1);
    QCOMPARE(toSpy.count(), 1);
    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.at(0), static_cast<QObject *>(kept.data()));
  }

  void dateRange_openEndedBoundLeavesThatSideUnrestricted() {
    const QUuid accountId = QUuid::createUuid();

    const TransactionPtr early =
        makeTransaction(QStringLiteral("Early"), accountId, QDate(2020, 1, 1));
    const TransactionPtr kept =
        makeTransaction(QStringLiteral("Kept"), accountId, QDate(2026, 1, 15));
    const TransactionPtr after =
        makeTransaction(QStringLiteral("After"), accountId, QDate(2026, 1, 21));

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(early));
    QVERIFY(sourceModel.addTransaction(kept));
    QVERIFY(sourceModel.addTransaction(after));

    AccountTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);

    // Only dateTo is set: everything up to and including it passes,
    // regardless of how far in the past.
    proxy.setDateTo(QDate(2026, 1, 20));

    QCOMPARE(proxy.rowCount(), 2);
    QVERIFY(proxy.dateFrom().isNull());
  }

  void supportFilter_showsOnlyMatchingSupports() {
    const QUuid accountId = QUuid::createUuid();

    const TransactionPtr kept = makeTransaction(
        QStringLiteral("Kept"), accountId, QDate(),
        OpenAccountEnums::Support::Cash);
    const TransactionPtr other = makeTransaction(
        QStringLiteral("Other"), accountId, QDate(),
        OpenAccountEnums::Support::Card);

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(kept));
    QVERIFY(sourceModel.addTransaction(other));

    AccountTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    proxy.setSupportFilter({OpenAccountEnums::Support::Cash});

    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.at(0), static_cast<QObject *>(kept.data()));
  }

  void categoryFilter_showsOnlyMatchingCategories() {
    const QUuid accountId = QUuid::createUuid();
    const QUuid keptCategory = QUuid::createUuid();
    const QUuid otherCategory = QUuid::createUuid();

    const TransactionPtr kept =
        makeTransaction(QStringLiteral("Kept"), accountId, QDate(),
                        OpenAccountEnums::Support::Other, keptCategory);
    const TransactionPtr other =
        makeTransaction(QStringLiteral("Other"), accountId, QDate(),
                        OpenAccountEnums::Support::Other, otherCategory);

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(kept));
    QVERIFY(sourceModel.addTransaction(other));

    AccountTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    proxy.setCategoryFilter({keptCategory});

    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.at(0), static_cast<QObject *>(kept.data()));
  }

  void descriptionFilter_matchesSubstringAnywhereInDescription() {
    const QUuid accountId = QUuid::createUuid();

    const TransactionPtr neoPizza =
        makeTransaction(QStringLiteral("neo pizza"), accountId);
    const TransactionPtr pizzaTravel =
        makeTransaction(QStringLiteral("pizza travel"), accountId);
    const TransactionPtr pizzalala =
        makeTransaction(QStringLiteral("pizzalala"), accountId);
    const TransactionPtr ohehpizza =
        makeTransaction(QStringLiteral("ohehpizza"), accountId);
    const TransactionPtr other =
        makeTransaction(QStringLiteral("Electric bill"), accountId);

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(neoPizza));
    QVERIFY(sourceModel.addTransaction(pizzaTravel));
    QVERIFY(sourceModel.addTransaction(pizzalala));
    QVERIFY(sourceModel.addTransaction(ohehpizza));
    QVERIFY(sourceModel.addTransaction(other));

    AccountTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    proxy.setDescriptionFilter(QStringLiteral("pizza"));

    QCOMPARE(proxy.rowCount(), 4);
  }

  void descriptionFilter_isCaseInsensitive() {
    const QUuid accountId = QUuid::createUuid();

    const TransactionPtr kept =
        makeTransaction(QStringLiteral("GROCERY shopping"), accountId);

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(kept));

    AccountTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    proxy.setDescriptionFilter(QStringLiteral("grocery"));

    QCOMPARE(proxy.rowCount(), 1);
  }

  void descriptionFilter_treatsUserInputAsLiteralText() {
    const QUuid accountId = QUuid::createUuid();

    const TransactionPtr literalMatch =
        makeTransaction(QStringLiteral("pizza (fresh)"), accountId);
    const TransactionPtr notARegexGroupMatch =
        makeTransaction(QStringLiteral("pizza fresh"), accountId);

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(literalMatch));
    QVERIFY(sourceModel.addTransaction(notARegexGroupMatch));

    AccountTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    proxy.setDescriptionFilter(QStringLiteral("pizza (fresh)"));

    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.at(0), static_cast<QObject *>(literalMatch.data()));
  }

  void combinedFilters_areAllApplied() {
    const QUuid accountId = QUuid::createUuid();
    const QDate keptDate(2026, 3, 1);
    const QUuid keptCategory = QUuid::createUuid();

    const TransactionPtr kept = makeTransaction(
        QStringLiteral("Grocery shopping"), accountId, keptDate,
        OpenAccountEnums::Support::Cash, keptCategory);
    const TransactionPtr wrongDate = makeTransaction(
        QStringLiteral("Grocery shopping"), accountId, QDate(2026, 3, 2),
        OpenAccountEnums::Support::Cash, keptCategory);
    const TransactionPtr wrongDescription = makeTransaction(
        QStringLiteral("Electric bill"), accountId, keptDate,
        OpenAccountEnums::Support::Cash, keptCategory);

    TransactionListModel sourceModel;
    QVERIFY(sourceModel.addTransaction(kept));
    QVERIFY(sourceModel.addTransaction(wrongDate));
    QVERIFY(sourceModel.addTransaction(wrongDescription));

    AccountTransactionFilterProxyModel proxy;
    proxy.setSourceModel(&sourceModel);
    proxy.setDateFilter({keptDate});
    proxy.setSupportFilter({OpenAccountEnums::Support::Cash});
    proxy.setCategoryFilter({keptCategory});
    proxy.setDescriptionFilter(QStringLiteral("Grocery"));

    QCOMPARE(proxy.rowCount(), 1);
    QCOMPARE(proxy.at(0), static_cast<QObject *>(kept.data()));
  }

  void settingSameFilter_doesNotEmitChanged() {
    AccountTransactionFilterProxyModel proxy;

    proxy.setDateFilter({QDate(2026, 1, 1)});
    QSignalSpy dateSpy(&proxy,
                       &AccountTransactionFilterProxyModel::dateFilterChanged);
    proxy.setDateFilter({QDate(2026, 1, 1)});
    QCOMPARE(dateSpy.count(), 0);

    proxy.setDescriptionFilter(QStringLiteral("foo"));
    QSignalSpy descriptionSpy(
        &proxy, &AccountTransactionFilterProxyModel::descriptionFilterChanged);
    proxy.setDescriptionFilter(QStringLiteral("foo"));
    QCOMPARE(descriptionSpy.count(), 0);

    proxy.setDateFrom(QDate(2026, 1, 1));
    QSignalSpy dateFromSpy(&proxy, &AccountTransactionFilterProxyModel::dateFromChanged);
    proxy.setDateFrom(QDate(2026, 1, 1));
    QCOMPARE(dateFromSpy.count(), 0);

    proxy.setDateTo(QDate(2026, 1, 31));
    QSignalSpy dateToSpy(&proxy, &AccountTransactionFilterProxyModel::dateToChanged);
    proxy.setDateTo(QDate(2026, 1, 31));
    QCOMPARE(dateToSpy.count(), 0);
  }
};

QTEST_MAIN(TestAccountTransactionFilterProxyModel)
#include "tst_accounttransactionfilterproxymodel.moc"
