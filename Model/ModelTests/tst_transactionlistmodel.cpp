#include "../include/Model/Business/transaction.h"
#include "../include/Model/Models/transactionlistmodel.h"

#include <QSignalSpy>
#include <QtTest/QtTest>

class TestTransactionListModel : public QObject {
  Q_OBJECT

private:
  static TransactionPtr makeTransaction(const QString &description, double value) {
    TransactionPtr transaction = TransactionPtr::create();
    transaction->setDescription(description);
    transaction->setValue(value);
    transaction->setSupport(OpenAccountEnums::Support::Card);
    transaction->setDate(QDate(2026, 1, 10));
    transaction->setMovement(OpenAccountEnums::Movement::Debit);
    transaction->setIsVisible(true);
    transaction->setCategory(QUuid::createUuid());
    return transaction;
  }

private slots:
  void defaultState_isEmpty() {
    TransactionListModel model;

    QCOMPARE(model.rowCount(), 0);
    QVERIFY(model.transactions().isEmpty());
    QVERIFY(!model.at(0));
    QVERIFY(model.get(0).isEmpty());
  }

  void addTransaction_preservesTransactionAccountId() {
    TransactionListModel model;

    const TransactionPtr transaction =
        makeTransaction(QStringLiteral("Groceries"), 42.5);
    const QUuid accountId = QUuid::createUuid();
    transaction->setAccountId(accountId);

    QSignalSpy rowsInsertedSpy(&model, &QAbstractItemModel::rowsInserted);
    QSignalSpy countSpy(&model, &TransactionListModel::countChanged);

    QVERIFY(model.addTransaction(transaction));

    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(transaction->accountId(), accountId);
    QCOMPARE(model.data(model.index(0, 0), TransactionListModel::ValueRole)
                 .toDouble(),
             42.5);
    QCOMPARE(model.data(model.index(0, 0), TransactionListModel::DescriptionRole)
                 .toString(),
             QStringLiteral("Groceries"));
    QCOMPARE(qvariant_cast<QObject *>(
                 model.data(model.index(0, 0), TransactionListModel::TransactionRole)),
             static_cast<QObject *>(transaction.data()));

    const QVariantMap payload = model.get(0);
    QCOMPARE(payload.value(QStringLiteral("description")).toString(),
             QStringLiteral("Groceries"));
    QCOMPARE(payload.value(QStringLiteral("accountId")).toUuid(), accountId);
  }

  void estimatedRole_roundTripsThroughDataAndSetData() {
    TransactionListModel model;
    const TransactionPtr transaction = makeTransaction(QStringLiteral("Insurance"), 60.0);
    QVERIFY(model.addTransaction(transaction));

    const QModelIndex index = model.index(0, 0);
    QCOMPARE(model.data(index, TransactionListModel::EstimatedRole).toBool(), false);

    QVERIFY(model.setData(index, true, TransactionListModel::EstimatedRole));
    QCOMPARE(transaction->estimated(), true);
    QCOMPARE(model.data(index, TransactionListModel::EstimatedRole).toBool(), true);
    QCOMPARE(model.get(0).value(QStringLiteral("estimated")).toBool(), true);
  }

  void estimatedChanges_emitDataChanged() {
    TransactionListModel model;
    const TransactionPtr transaction = makeTransaction(QStringLiteral("Phone bill"), 25.0);
    QVERIFY(model.addTransaction(transaction));

    QSignalSpy dataChangedSpy(&model, &QAbstractItemModel::dataChanged);
    transaction->setEstimated(true);

    QCOMPARE(dataChangedSpy.count(), 1);
    const QList<QVariant> arguments = dataChangedSpy.takeFirst();
    const QList<int> roles = qvariant_cast<QList<int>>(arguments.at(2));
    QVERIFY(roles.contains(TransactionListModel::EstimatedRole));
  }

  void removeTransaction_removesByIdAndReturnsFalseWhenNotFound() {
    TransactionListModel model;
    const TransactionPtr kept = makeTransaction(QStringLiteral("Kept"), 10.0);
    const TransactionPtr removed = makeTransaction(QStringLiteral("Removed"), 20.0);
    QVERIFY(model.addTransaction(kept));
    QVERIFY(model.addTransaction(removed));

    QSignalSpy rowsRemovedSpy(&model, &QAbstractItemModel::rowsRemoved);
    QSignalSpy countSpy(&model, &TransactionListModel::countChanged);

    QVERIFY(model.removeTransaction(removed->id()));
    QCOMPARE(rowsRemovedSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.at(0), static_cast<QObject *>(kept.data()));

    QVERIFY(!model.removeTransaction(removed->id())); // already gone
    QVERIFY(!model.removeTransaction(QUuid::createUuid())); // never existed
  }

  void duplicateTransactions_areRejected() {
    TransactionListModel model;
    const TransactionPtr transaction = makeTransaction(QStringLiteral("Rent"), 900.0);

    QVERIFY(model.addTransaction(transaction));
    QVERIFY(!model.addTransaction(transaction));

    const TransactionPtr duplicateId = makeTransaction(QStringLiteral("Rent copy"), 900.0);
    duplicateId->setId(transaction->id());
    QVERIFY(!model.addTransaction(duplicateId));
    QCOMPARE(model.rowCount(), 1);
  }

  void transactionChanges_emitDataChanged() {
    TransactionListModel model;
    const TransactionPtr transaction =
        makeTransaction(QStringLiteral("Initial"), 10.0);
    QVERIFY(model.addTransaction(transaction));

    QSignalSpy dataChangedSpy(&model, &QAbstractItemModel::dataChanged);

    transaction->setDescription(QStringLiteral("Updated"));

    QCOMPARE(dataChangedSpy.count(), 1);
    const QList<QVariant> arguments = dataChangedSpy.takeFirst();
    QCOMPARE(arguments.at(0).value<QModelIndex>().row(), 0);
    const QList<int> roles = qvariant_cast<QList<int>>(arguments.at(2));
    QVERIFY(roles.contains(TransactionListModel::DescriptionRole));
    QVERIFY(roles.contains(Qt::DisplayRole));
  }

  void changingTransactionAccountId_keepsItInTheModel() {
    TransactionListModel model;
    const TransactionPtr transaction =
        makeTransaction(QStringLiteral("Subscription"), 14.99);
    QVERIFY(model.addTransaction(transaction));

    QSignalSpy rowsRemovedSpy(&model, &QAbstractItemModel::rowsRemoved);
    QSignalSpy dataChangedSpy(&model, &QAbstractItemModel::dataChanged);

    transaction->setAccountId(QUuid::createUuid());

    QCOMPARE(rowsRemovedSpy.count(), 0);
    QCOMPARE(dataChangedSpy.count(), 1);
    QCOMPARE(model.rowCount(), 1);
  }

  void setTransactions_replacesContentAndSkipsInvalidEntries() {
    TransactionListModel model;

    const TransactionPtr first = makeTransaction(QStringLiteral("A"), 1.0);
    const TransactionPtr second = makeTransaction(QStringLiteral("B"), 2.0);
    const TransactionPtr duplicate = makeTransaction(QStringLiteral("C"), 3.0);
    duplicate->setId(first->id());

    QSignalSpy resetSpy(&model, &QAbstractItemModel::modelReset);
    QSignalSpy countSpy(&model, &TransactionListModel::countChanged);

    model.setTransactions({first, {}, second, first, duplicate});

    QCOMPARE(resetSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(model.rowCount(), 2);
  }
};

QTEST_MAIN(TestTransactionListModel)
#include "tst_transactionlistmodel.moc"
