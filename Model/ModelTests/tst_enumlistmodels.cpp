#include "Model/Models/frequencymodel.h"
#include "Model/Models/movementmodel.h"
#include "Model/Models/supportmodel.h"

#include <QtTest>

class TestEnumListModels : public QObject {
  Q_OBJECT

private slots:
  // --- MovementModel -------------------------------------------------------

  void movement_rowCountMatchesEnumEntries() {
    MovementModel model;
    QCOMPARE(model.rowCount(), 3);
  }

  void movement_dataExposesDisplayText() {
    MovementModel model;
    QCOMPARE(model.data(model.index(1), Qt::DisplayRole).toString(),
              QStringLiteral("Credit"));
  }

  void movement_valueAtMatchesIndexOfValue() {
    MovementModel model;
    for (int row = 0; row < model.rowCount(); ++row) {
      QCOMPARE(model.indexOfValue(model.valueAt(row)), row);
    }
  }

  void movement_indexOfValue_findsEachEnumerator() {
    MovementModel model;
    QVERIFY(model.indexOfValue(OpenAccountEnums::Movement::Both) >= 0);
    QVERIFY(model.indexOfValue(OpenAccountEnums::Movement::Credit) >= 0);
    QVERIFY(model.indexOfValue(OpenAccountEnums::Movement::Debit) >= 0);
  }

  void movement_textAt_matchesDisplayRoleData() {
    MovementModel model;
    const int row = model.indexOfValue(OpenAccountEnums::Movement::Credit);
    QCOMPARE(model.textAt(row), QStringLiteral("Credit"));
    QCOMPARE(model.textAt(row), model.data(model.index(row), Qt::DisplayRole).toString());
  }

  void textAt_outOfRangeReturnsEmpty() {
    MovementModel model;
    QVERIFY(model.textAt(-1).isEmpty());
    QVERIFY(model.textAt(model.rowCount()).isEmpty());
  }

  // --- SupportModel ----------------------------------------------------------

  void support_rowCountMatchesEnumEntries() {
    SupportModel model;
    QCOMPARE(model.rowCount(), 5);
  }

  void support_valueAtMatchesIndexOfValue() {
    SupportModel model;
    for (int row = 0; row < model.rowCount(); ++row) {
      QCOMPARE(model.indexOfValue(model.valueAt(row)), row);
    }
  }

  void support_indexOfValue_findsEachEnumerator() {
    SupportModel model;
    QVERIFY(model.indexOfValue(OpenAccountEnums::Support::Cash) >= 0);
    QVERIFY(model.indexOfValue(OpenAccountEnums::Support::Cheque) >= 0);
    QVERIFY(model.indexOfValue(OpenAccountEnums::Support::Card) >= 0);
    QVERIFY(model.indexOfValue(OpenAccountEnums::Support::Transfer) >= 0);
    QVERIFY(model.indexOfValue(OpenAccountEnums::Support::Other) >= 0);
  }

  // --- FrequencyModel --------------------------------------------------------

  void frequency_rowCountExcludesSentinelLast() {
    FrequencyModel model;
    QCOMPARE(model.rowCount(), 7);
  }

  void frequency_valueAtMatchesIndexOfValue() {
    FrequencyModel model;
    for (int row = 0; row < model.rowCount(); ++row) {
      QCOMPARE(model.indexOfValue(model.valueAt(row)), row);
    }
  }

  void frequency_indexOfValue_findsEachEnumerator() {
    FrequencyModel model;
    QVERIFY(model.indexOfValue(OpenAccountEnums::Frequency::Custom) >= 0);
    QVERIFY(model.indexOfValue(OpenAccountEnums::Frequency::Once) >= 0);
    QVERIFY(model.indexOfValue(OpenAccountEnums::Frequency::Daily) >= 0);
    QVERIFY(model.indexOfValue(OpenAccountEnums::Frequency::Weekly) >= 0);
    QVERIFY(model.indexOfValue(OpenAccountEnums::Frequency::Monthly) >= 0);
    QVERIFY(model.indexOfValue(OpenAccountEnums::Frequency::Quarterly) >= 0);
    QVERIFY(model.indexOfValue(OpenAccountEnums::Frequency::Yearly) >= 0);
  }

  // --- Shared EnumListModel behavior (checked once via MovementModel) --------

  void roleNames_exposeOnlyASingleTextRole() {
    MovementModel model;
    const QHash<int, QByteArray> roles = model.roleNames();
    QCOMPARE(roles.size(), 1);
    QCOMPARE(roles.value(Qt::DisplayRole), QByteArray("text"));
  }

  void data_outOfRangeReturnsInvalid() {
    MovementModel model;
    QVERIFY(!model.data(model.index(model.rowCount())).isValid());
  }

  void valueAt_outOfRangeReturnsMinusOne() {
    MovementModel model;
    QCOMPARE(static_cast<int>(model.valueAt(-1)), -1);
    QCOMPARE(static_cast<int>(model.valueAt(model.rowCount())), -1);
  }

  void indexOfValue_unknownValueReturnsMinusOne() {
    SupportModel model;
    QCOMPARE(model.indexOfValue(static_cast<OpenAccountEnums::Support>(-1)), -1);
  }
};

QTEST_MAIN(TestEnumListModels)
#include "tst_enumlistmodels.moc"
