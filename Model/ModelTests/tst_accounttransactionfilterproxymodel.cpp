#include "../include/Model/account.h"
#include "../include/Model/accounttransactionfilterproxymodel.h"
#include "../include/Model/enums.h"
#include "../include/Model/transaction.h"
#include "../include/Model/transactionlistmodel.h"

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
};

QTEST_MAIN(TestAccountTransactionFilterProxyModel)
#include "tst_accounttransactionfilterproxymodel.moc"
