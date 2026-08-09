#include "../include/Model/Business/tricount.h"
#include "../include/Model/Models/tricountlistmodel.h"

#include <QSignalSpy>
#include <QtTest/QtTest>

class TestTricountListModel : public QObject {
  Q_OBJECT

private:
  static TricountPtr makeTricount(const QString &name) {
    TricountPtr tricount = TricountPtr::create();
    tricount->setId(QUuid::createUuid());
    tricount->setName(name);
    return tricount;
  }

private slots:
  void defaultState_isEmpty() {
    TricountListModel model;

    QCOMPARE(model.rowCount(), 0);
    QVERIFY(model.tricounts().isEmpty());
    QVERIFY(!model.at(0));
    QVERIFY(model.get(0).isEmpty());
  }

  void addTricount_exposesNameAndCurrency() {
    TricountListModel model;

    const TricountPtr tricount = makeTricount(QStringLiteral("Ski trip"));
    tricount->setCurrency(QStringLiteral("EUR"));

    QSignalSpy rowsInsertedSpy(&model, &QAbstractItemModel::rowsInserted);
    QSignalSpy countSpy(&model, &TricountListModel::countChanged);

    QVERIFY(model.addTricount(tricount));

    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.data(model.index(0, 0), TricountListModel::NameRole).toString(),
             QStringLiteral("Ski trip"));
    QCOMPARE(model.data(model.index(0, 0), TricountListModel::CurrencyRole).toString(),
             QStringLiteral("EUR"));
    QCOMPARE(qvariant_cast<QObject *>(
                 model.data(model.index(0, 0), TricountListModel::TricountRole)),
             static_cast<QObject *>(tricount.data()));

    const QVariantMap payload = model.get(0);
    QCOMPARE(payload.value(QStringLiteral("name")).toString(), QStringLiteral("Ski trip"));
  }

  void duplicateTricounts_areRejected() {
    TricountListModel model;
    const TricountPtr tricount = makeTricount(QStringLiteral("Trip A"));

    QVERIFY(model.addTricount(tricount));
    QVERIFY(!model.addTricount(tricount));

    const TricountPtr duplicateId = makeTricount(QStringLiteral("Trip A copy"));
    duplicateId->setId(tricount->id());
    QVERIFY(!model.addTricount(duplicateId));
    QCOMPARE(model.rowCount(), 1);
  }

  void tricountChanges_emitDataChanged() {
    TricountListModel model;
    const TricountPtr tricount = makeTricount(QStringLiteral("Initial"));
    QVERIFY(model.addTricount(tricount));

    QSignalSpy dataChangedSpy(&model, &QAbstractItemModel::dataChanged);

    tricount->setName(QStringLiteral("Updated"));

    QCOMPARE(dataChangedSpy.count(), 1);
    const QList<QVariant> arguments = dataChangedSpy.takeFirst();
    QCOMPARE(arguments.at(0).value<QModelIndex>().row(), 0);
    const QList<int> roles = qvariant_cast<QList<int>>(arguments.at(2));
    QVERIFY(roles.contains(TricountListModel::NameRole));
    QVERIFY(roles.contains(Qt::DisplayRole));
  }

  void indexOf_findsTricountById() {
    TricountListModel model;
    const TricountPtr first = makeTricount(QStringLiteral("A"));
    const TricountPtr second = makeTricount(QStringLiteral("B"));
    model.setTricounts({first, second});

    QCOMPARE(model.indexOf(first->id()), 0);
    QCOMPARE(model.indexOf(second->id()), 1);
    QCOMPARE(model.indexOf(QUuid::createUuid()), -1);
  }

  void removeTricount_shrinksModel() {
    TricountListModel model;
    const TricountPtr tricount = makeTricount(QStringLiteral("Trip"));
    QVERIFY(model.addTricount(tricount));

    QSignalSpy countSpy(&model, &TricountListModel::countChanged);

    QVERIFY(model.removeTricount(tricount->id()));
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(countSpy.count(), 1);
    QVERIFY(!model.removeTricount(tricount->id()));
  }

  void setTricounts_replacesContentAndSkipsInvalidEntries() {
    TricountListModel model;

    const TricountPtr first = makeTricount(QStringLiteral("A"));
    const TricountPtr second = makeTricount(QStringLiteral("B"));
    const TricountPtr duplicate = makeTricount(QStringLiteral("C"));
    duplicate->setId(first->id());

    QSignalSpy resetSpy(&model, &QAbstractItemModel::modelReset);
    QSignalSpy countSpy(&model, &TricountListModel::countChanged);

    model.setTricounts({first, {}, second, first, duplicate});

    QCOMPARE(resetSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(model.rowCount(), 2);
  }

  void clear_removesEveryTricount() {
    TricountListModel model;
    model.setTricounts({makeTricount(QStringLiteral("A")), makeTricount(QStringLiteral("B"))});

    QSignalSpy countSpy(&model, &TricountListModel::countChanged);
    model.clear();

    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(countSpy.count(), 1);
  }
};

QTEST_MAIN(TestTricountListModel)
#include "tst_tricountlistmodel.moc"
