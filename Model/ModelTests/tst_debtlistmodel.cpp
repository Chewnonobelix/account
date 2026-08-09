#include "../include/Model/Business/debt.h"
#include "../include/Model/Models/debtlistmodel.h"

#include <QSignalSpy>
#include <QtTest/QtTest>

class TestDebtListModel : public QObject {
  Q_OBJECT

private:
  static DebtPtr makeDebt(const QString &name, double principal) {
    DebtPtr debt = DebtPtr::create();
    debt->setId(QUuid::createUuid());
    debt->setName(name);
    debt->setPrincipal(principal);
    return debt;
  }

private slots:
  void defaultState_isEmpty() {
    DebtListModel model;

    QCOMPARE(model.rowCount(), 0);
    QVERIFY(model.debts().isEmpty());
    QVERIFY(!model.at(0));
    QVERIFY(model.get(0).isEmpty());
  }

  void addDebt_exposesNameAndAccountId() {
    DebtListModel model;

    const QUuid accountId = QUuid::createUuid();
    const DebtPtr debt = makeDebt(QStringLiteral("Car loan"), 15000.0);
    debt->setAccountId(accountId);

    QSignalSpy rowsInsertedSpy(&model, &QAbstractItemModel::rowsInserted);
    QSignalSpy countSpy(&model, &DebtListModel::countChanged);

    QVERIFY(model.addDebt(debt));

    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.data(model.index(0, 0), DebtListModel::NameRole).toString(),
             QStringLiteral("Car loan"));
    QCOMPARE(model.data(model.index(0, 0), DebtListModel::AccountIdRole).toUuid(),
             accountId);
    QCOMPARE(qvariant_cast<QObject *>(
                 model.data(model.index(0, 0), DebtListModel::DebtRole)),
             static_cast<QObject *>(debt.data()));

    const QVariantMap payload = model.get(0);
    QCOMPARE(payload.value(QStringLiteral("name")).toString(), QStringLiteral("Car loan"));
  }

  void duplicateDebts_areRejected() {
    DebtListModel model;
    const DebtPtr debt = makeDebt(QStringLiteral("Loan A"), 5000.0);

    QVERIFY(model.addDebt(debt));
    QVERIFY(!model.addDebt(debt));

    const DebtPtr duplicateId = makeDebt(QStringLiteral("Loan A copy"), 5000.0);
    duplicateId->setId(debt->id());
    QVERIFY(!model.addDebt(duplicateId));
    QCOMPARE(model.rowCount(), 1);
  }

  void debtChanges_emitDataChanged() {
    DebtListModel model;
    const DebtPtr debt = makeDebt(QStringLiteral("Initial"), 100.0);
    QVERIFY(model.addDebt(debt));

    QSignalSpy dataChangedSpy(&model, &QAbstractItemModel::dataChanged);

    debt->setName(QStringLiteral("Updated"));

    QCOMPARE(dataChangedSpy.count(), 1);
    const QList<QVariant> arguments = dataChangedSpy.takeFirst();
    QCOMPARE(arguments.at(0).value<QModelIndex>().row(), 0);
    const QList<int> roles = qvariant_cast<QList<int>>(arguments.at(2));
    QVERIFY(roles.contains(DebtListModel::NameRole));
    QVERIFY(roles.contains(Qt::DisplayRole));
  }

  void indexOf_findsDebtById() {
    DebtListModel model;
    const DebtPtr first = makeDebt(QStringLiteral("A"), 10.0);
    const DebtPtr second = makeDebt(QStringLiteral("B"), 20.0);
    model.setDebts({first, second});

    QCOMPARE(model.indexOf(first->id()), 0);
    QCOMPARE(model.indexOf(second->id()), 1);
    QCOMPARE(model.indexOf(QUuid::createUuid()), -1);
  }

  void removeDebt_shrinksModel() {
    DebtListModel model;
    const DebtPtr debt = makeDebt(QStringLiteral("Loan"), 1000.0);
    QVERIFY(model.addDebt(debt));

    QSignalSpy countSpy(&model, &DebtListModel::countChanged);

    QVERIFY(model.removeDebt(debt->id()));
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(countSpy.count(), 1);
    QVERIFY(!model.removeDebt(debt->id()));
  }

  void setDebts_replacesContentAndSkipsInvalidEntries() {
    DebtListModel model;

    const DebtPtr first = makeDebt(QStringLiteral("A"), 10.0);
    const DebtPtr second = makeDebt(QStringLiteral("B"), 20.0);
    const DebtPtr duplicate = makeDebt(QStringLiteral("C"), 30.0);
    duplicate->setId(first->id());

    QSignalSpy resetSpy(&model, &QAbstractItemModel::modelReset);
    QSignalSpy countSpy(&model, &DebtListModel::countChanged);

    model.setDebts({first, {}, second, first, duplicate});

    QCOMPARE(resetSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(model.rowCount(), 2);
  }

  void clear_removesEveryDebt() {
    DebtListModel model;
    model.setDebts({makeDebt(QStringLiteral("A"), 10.0),
                     makeDebt(QStringLiteral("B"), 20.0)});

    QSignalSpy countSpy(&model, &DebtListModel::countChanged);
    model.clear();

    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(countSpy.count(), 1);
  }
};

QTEST_MAIN(TestDebtListModel)
#include "tst_debtlistmodel.moc"
