#include "../include/Model/Business/frequency.h"
#include "../include/Model/Business/transaction.h"
#include "../include/Model/Models/frequencylistmodel.h"

#include <QSignalSpy>
#include <QtTest/QtTest>

class TestFrequencyListModel : public QObject {
  Q_OBJECT

private:
  static FrequencyPtr makeFrequency(OpenAccountEnums::Frequency interval) {
    FrequencyPtr frequency = FrequencyPtr::create();
    frequency->setId(QUuid::createUuid());
    frequency->setFrequency(interval);
    return frequency;
  }

private slots:
  void defaultState_isEmpty() {
    FrequencyListModel model;

    QCOMPARE(model.rowCount(), 0);
    QVERIFY(model.frequencies().isEmpty());
    QVERIFY(!model.at(0));
    QVERIFY(model.get(0).isEmpty());
  }

  void addFrequency_exposesIntervalAndPrototype() {
    FrequencyListModel model;

    const FrequencyPtr frequency = makeFrequency(OpenAccountEnums::Frequency::Monthly);
    const TransactionPtr prototype = TransactionPtr::create();
    prototype->setAccountId(QUuid::createUuid());
    frequency->setPrototype(prototype);

    QSignalSpy rowsInsertedSpy(&model, &QAbstractItemModel::rowsInserted);
    QSignalSpy countSpy(&model, &FrequencyListModel::countChanged);

    QVERIFY(model.addFrequency(frequency));

    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.data(model.index(0, 0), FrequencyListModel::IntervalRole)
                 .value<OpenAccountEnums::Frequency>(),
             OpenAccountEnums::Frequency::Monthly);
    QCOMPARE(model.data(model.index(0, 0), FrequencyListModel::PrototypeRole)
                 .value<TransactionPtr>(),
             prototype);
    QCOMPARE(qvariant_cast<QObject *>(
                 model.data(model.index(0, 0), FrequencyListModel::FrequencyRole)),
             static_cast<QObject *>(frequency.data()));
  }

  void duplicateFrequencies_areRejected() {
    FrequencyListModel model;
    const FrequencyPtr frequency = makeFrequency(OpenAccountEnums::Frequency::Weekly);

    QVERIFY(model.addFrequency(frequency));
    QVERIFY(!model.addFrequency(frequency));

    const FrequencyPtr duplicateId = makeFrequency(OpenAccountEnums::Frequency::Weekly);
    duplicateId->setId(frequency->id());
    QVERIFY(!model.addFrequency(duplicateId));
    QCOMPARE(model.rowCount(), 1);
  }

  void frequencyChanges_emitDataChanged() {
    FrequencyListModel model;
    const FrequencyPtr frequency = makeFrequency(OpenAccountEnums::Frequency::Once);
    QVERIFY(model.addFrequency(frequency));

    QSignalSpy dataChangedSpy(&model, &QAbstractItemModel::dataChanged);

    frequency->setFrequency(OpenAccountEnums::Frequency::Yearly);

    QCOMPARE(dataChangedSpy.count(), 1);
    const QList<QVariant> arguments = dataChangedSpy.takeFirst();
    QCOMPARE(arguments.at(0).value<QModelIndex>().row(), 0);
    const QList<int> roles = qvariant_cast<QList<int>>(arguments.at(2));
    QVERIFY(roles.contains(FrequencyListModel::IntervalRole));
  }

  void indexOf_findsFrequencyById() {
    FrequencyListModel model;
    const FrequencyPtr first = makeFrequency(OpenAccountEnums::Frequency::Daily);
    const FrequencyPtr second = makeFrequency(OpenAccountEnums::Frequency::Daily);
    model.setFrequencies({first, second});

    QCOMPARE(model.indexOf(first->id()), 0);
    QCOMPARE(model.indexOf(second->id()), 1);
    QCOMPARE(model.indexOf(QUuid::createUuid()), -1);
  }

  void removeFrequency_shrinksModel() {
    FrequencyListModel model;
    const FrequencyPtr frequency = makeFrequency(OpenAccountEnums::Frequency::Daily);
    QVERIFY(model.addFrequency(frequency));

    QSignalSpy countSpy(&model, &FrequencyListModel::countChanged);

    QVERIFY(model.removeFrequency(frequency->id()));
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(countSpy.count(), 1);
    QVERIFY(!model.removeFrequency(frequency->id()));
  }

  void setFrequencies_replacesContentAndSkipsInvalidEntries() {
    FrequencyListModel model;

    const FrequencyPtr first = makeFrequency(OpenAccountEnums::Frequency::Daily);
    const FrequencyPtr second = makeFrequency(OpenAccountEnums::Frequency::Daily);
    const FrequencyPtr duplicate = makeFrequency(OpenAccountEnums::Frequency::Daily);
    duplicate->setId(first->id());

    QSignalSpy resetSpy(&model, &QAbstractItemModel::modelReset);
    QSignalSpy countSpy(&model, &FrequencyListModel::countChanged);

    model.setFrequencies({first, {}, second, first, duplicate});

    QCOMPARE(resetSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(model.rowCount(), 2);
  }

  void clear_removesEveryFrequency() {
    FrequencyListModel model;
    model.setFrequencies({makeFrequency(OpenAccountEnums::Frequency::Daily),
                          makeFrequency(OpenAccountEnums::Frequency::Weekly)});

    QSignalSpy countSpy(&model, &FrequencyListModel::countChanged);
    model.clear();

    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(countSpy.count(), 1);
  }
};

QTEST_MAIN(TestFrequencyListModel)
#include "tst_frequencylistmodel.moc"
