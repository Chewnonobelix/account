#include "../include/Model/Business/budget.h"
#include "../include/Model/Models/budgetlistmodel.h"

#include <QSignalSpy>
#include <QtTest/QtTest>

class TestBudgetListModel : public QObject {
  Q_OBJECT

private:
  static BudgetPtr makeBudget(const QString &name, double threshold) {
    BudgetPtr budget = BudgetPtr::create();
    budget->setId(QUuid::createUuid());
    budget->setName(name);
    budget->setThreshold(threshold);
    return budget;
  }

private slots:
  void defaultState_isEmpty() {
    BudgetListModel model;

    QCOMPARE(model.rowCount(), 0);
    QVERIFY(model.budgets().isEmpty());
    QVERIFY(!model.at(0));
    QVERIFY(model.get(0).isEmpty());
  }

  void addBudget_exposesNameAndThreshold() {
    BudgetListModel model;

    const BudgetPtr budget = makeBudget(QStringLiteral("Groceries"), 300.0);

    QSignalSpy rowsInsertedSpy(&model, &QAbstractItemModel::rowsInserted);
    QSignalSpy countSpy(&model, &BudgetListModel::countChanged);

    QVERIFY(model.addBudget(budget));

    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.data(model.index(0, 0), BudgetListModel::NameRole).toString(),
             QStringLiteral("Groceries"));
    QCOMPARE(model.data(model.index(0, 0), BudgetListModel::ThresholdRole).toDouble(),
             300.0);
    QCOMPARE(qvariant_cast<QObject *>(
                 model.data(model.index(0, 0), BudgetListModel::BudgetRole)),
             static_cast<QObject *>(budget.data()));

    const QVariantMap payload = model.get(0);
    QCOMPARE(payload.value(QStringLiteral("name")).toString(), QStringLiteral("Groceries"));
  }

  void duplicateBudgets_areRejected() {
    BudgetListModel model;
    const BudgetPtr budget = makeBudget(QStringLiteral("Rent"), 1000.0);

    QVERIFY(model.addBudget(budget));
    QVERIFY(!model.addBudget(budget));

    const BudgetPtr duplicateId = makeBudget(QStringLiteral("Rent copy"), 1000.0);
    duplicateId->setId(budget->id());
    QVERIFY(!model.addBudget(duplicateId));
    QCOMPARE(model.rowCount(), 1);
  }

  void budgetChanges_emitDataChanged() {
    BudgetListModel model;
    const BudgetPtr budget = makeBudget(QStringLiteral("Initial"), 100.0);
    QVERIFY(model.addBudget(budget));

    QSignalSpy dataChangedSpy(&model, &QAbstractItemModel::dataChanged);

    budget->setName(QStringLiteral("Updated"));

    QCOMPARE(dataChangedSpy.count(), 1);
    const QList<QVariant> arguments = dataChangedSpy.takeFirst();
    QCOMPARE(arguments.at(0).value<QModelIndex>().row(), 0);
    const QList<int> roles = qvariant_cast<QList<int>>(arguments.at(2));
    QVERIFY(roles.contains(BudgetListModel::NameRole));
    QVERIFY(roles.contains(Qt::DisplayRole));
  }

  void indexOf_findsBudgetById() {
    BudgetListModel model;
    const BudgetPtr first = makeBudget(QStringLiteral("A"), 10.0);
    const BudgetPtr second = makeBudget(QStringLiteral("B"), 20.0);
    model.setBudgets({first, second});

    QCOMPARE(model.indexOf(first->id()), 0);
    QCOMPARE(model.indexOf(second->id()), 1);
    QCOMPARE(model.indexOf(QUuid::createUuid()), -1);
  }

  void removeBudget_shrinksModel() {
    BudgetListModel model;
    const BudgetPtr budget = makeBudget(QStringLiteral("Rent"), 1000.0);
    QVERIFY(model.addBudget(budget));

    QSignalSpy countSpy(&model, &BudgetListModel::countChanged);

    QVERIFY(model.removeBudget(budget->id()));
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(countSpy.count(), 1);
    QVERIFY(!model.removeBudget(budget->id()));
  }

  void setBudgets_replacesContentAndSkipsInvalidEntries() {
    BudgetListModel model;

    const BudgetPtr first = makeBudget(QStringLiteral("A"), 10.0);
    const BudgetPtr second = makeBudget(QStringLiteral("B"), 20.0);
    const BudgetPtr duplicate = makeBudget(QStringLiteral("C"), 30.0);
    duplicate->setId(first->id());

    QSignalSpy resetSpy(&model, &QAbstractItemModel::modelReset);
    QSignalSpy countSpy(&model, &BudgetListModel::countChanged);

    model.setBudgets({first, {}, second, first, duplicate});

    QCOMPARE(resetSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(model.rowCount(), 2);
  }

  void clear_removesEveryBudget() {
    BudgetListModel model;
    model.setBudgets({makeBudget(QStringLiteral("A"), 10.0),
                       makeBudget(QStringLiteral("B"), 20.0)});

    QSignalSpy countSpy(&model, &BudgetListModel::countChanged);
    model.clear();

    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(countSpy.count(), 1);
  }
};

QTEST_MAIN(TestBudgetListModel)
#include "tst_budgetlistmodel.moc"
