#include <QSignalSpy>
#include <QtTest>

#include "Model/frequency.h"

class TestFrequency : public QObject {
  Q_OBJECT

private slots:
  void defaultValues();
  void frequencyPtrMetaType();
  void setId_emitsOnlyOnChange();
  void setFrequency_emitsOnlyOnChange();
  void setDateFormat_emitsOnlyOnChange();
  void setDateFormat_emptyFallsBackToDefault();
  void setCustomIntervalDays_emitsOnlyOnChange();
  void setCustomIntervalDays_invalidFallsBackToOne();
  void setPrototype_emitsOnlyOnChange();

  void toJson_roundTrip();
  void constructorFromJson_withInvalidValuesFallsBackToDefaults();

  void generate_withoutPrototype_returnsEmpty();
  void generate_invalidRange_returnsEmpty();
  void generate_invalidCount_returnsEmpty();
  void generatedTransactions_haveDistinctIds();

  void generate_onceRange();
  void generate_dailyRange();
  void generate_weeklyRange();
  void generate_monthlyRange();
  void generate_quarterlyRange();
  void generate_yearlyRange();
  void generate_customRange();

  void generate_onceCount();
  void generate_dailyCount();
  void generate_customCount();

  void generatedTransactions_clonePrototypeData();
  void generatedTransactions_overrideDateAndDescription();
  void generatedTransactions_storeFrequencyIdMetadata();
  void generatedTransactions_frequencyIdMatchesSourceId();
  void generatedTransactions_distinctSharedPointers();
  void generatedTransactions_distinctIdsFromPrototype();
  void generatedTransactions_descriptionWithEmptyPrototype();
  void generatedTransactions_customDateFormat();
  void generatedTransactions_keepPrototypeUnchanged();
  void generatedTransactions_respectMonthBoundaries();
  void sharedFromThis_returnsSameInstance();

private:
  static TransactionPtr makePrototype();
  static QList<QDate> extractDates(const QList<TransactionPtr> &transactions);
};

TransactionPtr TestFrequency::makePrototype() {
  TransactionPtr prototype = TransactionPtr::create();
  prototype->setId(QUuid::createUuid());
  prototype->setValue(42.5);
  prototype->setDescription(QStringLiteral("Rent"));
  prototype->setSupport(OpenAccountEnums::Support::Transfer);
  prototype->setDate(QDate(2020, 1, 1));
  prototype->setMovement(OpenAccountEnums::Movement::Debit);
  prototype->setIsVisible(false);
  prototype->setAccountId(QUuid::createUuid());
  prototype->setCategory(QUuid::createUuid());
  prototype->setMetadata(QStringLiteral("customMarker"),
                         QStringLiteral("prototype"));
  return prototype;
}

QList<QDate>
TestFrequency::extractDates(const QList<TransactionPtr> &transactions) {
  QList<QDate> dates;
  for (const TransactionPtr &transaction : transactions)
    dates.append(transaction->date());
  return dates;
}

void TestFrequency::defaultValues() {
  Frequency frequency;

  QVERIFY(!frequency.id().isNull());
  QCOMPARE(frequency.frequency(), OpenAccountEnums::Frequency::Once);
  QCOMPARE(frequency.dateFormat(), QStringLiteral("yyyy-MM-dd"));
  QCOMPARE(frequency.customIntervalDays(), 1);
  QVERIFY(!frequency.prototype());
}

void TestFrequency::frequencyPtrMetaType() {
  const int typeId = qMetaTypeId<FrequencyPtr>();
  QVERIFY(typeId != QMetaType::UnknownType);

  FrequencyPtr frequency = FrequencyPtr::create();
  QVariant value = QVariant::fromValue(frequency);

  QVERIFY(value.isValid());
  QCOMPARE(value.value<FrequencyPtr>(), frequency);
}

void TestFrequency::setId_emitsOnlyOnChange() {
  Frequency frequency;
  QSignalSpy spy(&frequency, SIGNAL(idChanged()));

  const QUuid first = QUuid::createUuid();
  const QUuid second = QUuid::createUuid();

  frequency.setId(first);
  QCOMPARE(frequency.id(), first);
  QCOMPARE(spy.count(), 1);

  frequency.setId(first);
  QCOMPARE(spy.count(), 1);

  frequency.setId(second);
  QCOMPARE(frequency.id(), second);
  QCOMPARE(spy.count(), 2);
}

void TestFrequency::setFrequency_emitsOnlyOnChange() {
  Frequency frequency;
  QSignalSpy spy(&frequency, SIGNAL(frequencyChanged()));

  frequency.setFrequency(OpenAccountEnums::Frequency::Monthly);
  QCOMPARE(frequency.frequency(), OpenAccountEnums::Frequency::Monthly);
  QCOMPARE(spy.count(), 1);

  frequency.setFrequency(OpenAccountEnums::Frequency::Monthly);
  QCOMPARE(spy.count(), 1);

  frequency.setFrequency(OpenAccountEnums::Frequency::Yearly);
  QCOMPARE(frequency.frequency(), OpenAccountEnums::Frequency::Yearly);
  QCOMPARE(spy.count(), 2);
}

void TestFrequency::setDateFormat_emitsOnlyOnChange() {
  Frequency frequency;
  QSignalSpy spy(&frequency, SIGNAL(dateFormatChanged()));

  frequency.setDateFormat(QStringLiteral("dd/MM/yyyy"));
  QCOMPARE(frequency.dateFormat(), QStringLiteral("dd/MM/yyyy"));
  QCOMPARE(spy.count(), 1);

  frequency.setDateFormat(QStringLiteral("dd/MM/yyyy"));
  QCOMPARE(spy.count(), 1);

  frequency.setDateFormat(QStringLiteral("MM-yyyy"));
  QCOMPARE(frequency.dateFormat(), QStringLiteral("MM-yyyy"));
  QCOMPARE(spy.count(), 2);
}

void TestFrequency::setDateFormat_emptyFallsBackToDefault() {
  Frequency frequency;
  QSignalSpy spy(&frequency, SIGNAL(dateFormatChanged()));

  frequency.setDateFormat(QStringLiteral("dd/MM/yyyy"));
  QCOMPARE(spy.count(), 1);

  frequency.setDateFormat(QString());
  QCOMPARE(frequency.dateFormat(), QStringLiteral("yyyy-MM-dd"));
  QCOMPARE(spy.count(), 2);

  frequency.setDateFormat(QString());
  QCOMPARE(spy.count(), 2);
}

void TestFrequency::setCustomIntervalDays_emitsOnlyOnChange() {
  Frequency frequency;
  QSignalSpy spy(&frequency, SIGNAL(customIntervalDaysChanged()));

  frequency.setCustomIntervalDays(10);
  QCOMPARE(frequency.customIntervalDays(), 10);
  QCOMPARE(spy.count(), 1);

  frequency.setCustomIntervalDays(10);
  QCOMPARE(spy.count(), 1);

  frequency.setCustomIntervalDays(11);
  QCOMPARE(frequency.customIntervalDays(), 11);
  QCOMPARE(spy.count(), 2);
}

void TestFrequency::setCustomIntervalDays_invalidFallsBackToOne() {
  Frequency frequency;
  QSignalSpy spy(&frequency, SIGNAL(customIntervalDaysChanged()));

  frequency.setCustomIntervalDays(0);
  QCOMPARE(frequency.customIntervalDays(), 1);
  QCOMPARE(spy.count(), 0);

  frequency.setCustomIntervalDays(-5);
  QCOMPARE(frequency.customIntervalDays(), 1);
  QCOMPARE(spy.count(), 0);

  frequency.setCustomIntervalDays(2);
  QCOMPARE(frequency.customIntervalDays(), 2);
  QCOMPARE(spy.count(), 1);

  frequency.setCustomIntervalDays(-1);
  QCOMPARE(frequency.customIntervalDays(), 1);
  QCOMPARE(spy.count(), 2);
}

void TestFrequency::setPrototype_emitsOnlyOnChange() {
  Frequency frequency;
  QSignalSpy spy(&frequency, SIGNAL(prototypeChanged()));

  const TransactionPtr first = makePrototype();
  const TransactionPtr second = makePrototype();

  frequency.setPrototype(first);
  QCOMPARE(frequency.prototype(), first);
  QCOMPARE(spy.count(), 1);

  frequency.setPrototype(first);
  QCOMPARE(spy.count(), 1);

  frequency.setPrototype(second);
  QCOMPARE(frequency.prototype(), second);
  QCOMPARE(spy.count(), 2);
}

void TestFrequency::toJson_roundTrip() {
  Frequency frequency;
  const QUuid uid = QUuid::createUuid();
  const TransactionPtr prototype = makePrototype();

  frequency.setId(uid);
  frequency.setFrequency(OpenAccountEnums::Frequency::Custom);
  frequency.setDateFormat(QStringLiteral("dd/MM/yyyy"));
  frequency.setCustomIntervalDays(12);
  frequency.setPrototype(prototype);

  const QJsonObject json = frequency.toJson();
  Frequency restored(json);

  QCOMPARE(restored.id(), uid);
  QCOMPARE(restored.frequency(), OpenAccountEnums::Frequency::Custom);
  QCOMPARE(restored.dateFormat(), QStringLiteral("dd/MM/yyyy"));
  QCOMPARE(restored.customIntervalDays(), 12);
  QVERIFY(restored.prototype());
  QCOMPARE(restored.prototype()->value(), prototype->value());
  QCOMPARE(restored.prototype()->description(), prototype->description());
  QCOMPARE(restored.prototype()->support(), prototype->support());
  QCOMPARE(restored.prototype()->date(), prototype->date());
  QCOMPARE(restored.prototype()->movement(), prototype->movement());
  QCOMPARE(restored.prototype()->isVisible(), prototype->isVisible());
  QCOMPARE(restored.prototype()->accountId(), prototype->accountId());
  QCOMPARE(restored.prototype()->category(), prototype->category());
}

void TestFrequency::constructorFromJson_withInvalidValuesFallsBackToDefaults() {
  QJsonObject json;
  json.insert(QStringLiteral("dateFormat"), QString());
  json.insert(QStringLiteral("customIntervalDays"), 0);
  json.insert(QStringLiteral("frequency"),
              static_cast<int>(OpenAccountEnums::Frequency::Daily));

  Frequency frequency(json);

  QVERIFY(!frequency.id().isNull());
  QCOMPARE(frequency.frequency(), OpenAccountEnums::Frequency::Daily);
  QCOMPARE(frequency.dateFormat(), QStringLiteral("yyyy-MM-dd"));
  QCOMPARE(frequency.customIntervalDays(), 1);
  QVERIFY(!frequency.prototype());
}

void TestFrequency::generate_withoutPrototype_returnsEmpty() {
  Frequency frequency;
  QCOMPARE(frequency.generate(QDate(2026, 1, 1), QDate(2026, 1, 10)).size(), 0);
  QCOMPARE(frequency.generate(QDate(2026, 1, 1), 3).size(), 0);
}

void TestFrequency::generate_invalidRange_returnsEmpty() {
  Frequency frequency;
  frequency.setPrototype(makePrototype());

  QCOMPARE(frequency.generate(QDate(), QDate(2026, 1, 10)).size(), 0);
  QCOMPARE(frequency.generate(QDate(2026, 1, 10), QDate()).size(), 0);
  QCOMPARE(frequency.generate(QDate(2026, 1, 10), QDate(2026, 1, 1)).size(), 0);
}

void TestFrequency::generate_invalidCount_returnsEmpty() {
  Frequency frequency;
  frequency.setPrototype(makePrototype());

  QCOMPARE(frequency.generate(QDate(), 3).size(), 0);
  QCOMPARE(frequency.generate(QDate(2026, 1, 1), 0).size(), 0);
  QCOMPARE(frequency.generate(QDate(2026, 1, 1), -1).size(), 0);
}

void TestFrequency::generate_onceRange() {
  Frequency frequency;
  frequency.setPrototype(makePrototype());
  frequency.setFrequency(OpenAccountEnums::Frequency::Once);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 1, 1), QDate(2026, 1, 31));

  QCOMPARE(transactions.size(), 1);
  QCOMPARE(extractDates(transactions), QList<QDate>{QDate(2026, 1, 1)});
}

void TestFrequency::generate_dailyRange() {
  Frequency frequency;
  frequency.setPrototype(makePrototype());
  frequency.setFrequency(OpenAccountEnums::Frequency::Daily);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 1, 1), QDate(2026, 1, 3));

  QCOMPARE(transactions.size(), 3);
  QList<QDate> expected{QDate(2026, 1, 1), QDate(2026, 1, 2),
                        QDate(2026, 1, 3)};
  QCOMPARE(extractDates(transactions), expected);
}

void TestFrequency::generate_weeklyRange() {
  Frequency frequency;
  frequency.setPrototype(makePrototype());
  frequency.setFrequency(OpenAccountEnums::Frequency::Weekly);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 1, 1), QDate(2026, 1, 20));

  QCOMPARE(transactions.size(), 3);
  QList<QDate> expected{QDate(2026, 1, 1), QDate(2026, 1, 8),
                        QDate(2026, 1, 15)};
  QCOMPARE(extractDates(transactions), expected);
}

void TestFrequency::generate_monthlyRange() {
  Frequency frequency;
  frequency.setPrototype(makePrototype());
  frequency.setFrequency(OpenAccountEnums::Frequency::Monthly);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 1, 31), QDate(2026, 5, 31));

  QCOMPARE(transactions.size(), 5);
  QList<QDate> expected{QDate(2026, 1, 31), QDate(2026, 2, 28),
                        QDate(2026, 3, 28), QDate(2026, 4, 28),
                        QDate(2026, 5, 28)};
  QCOMPARE(extractDates(transactions), expected);
}

void TestFrequency::generate_quarterlyRange() {
  Frequency frequency;
  frequency.setPrototype(makePrototype());
  frequency.setFrequency(OpenAccountEnums::Frequency::Quarterly);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 1, 31), QDate(2026, 12, 31));

  QCOMPARE(transactions.size(), 4);
  QList<QDate> expected{QDate(2026, 1, 31), QDate(2026, 4, 30),
                        QDate(2026, 7, 30), QDate(2026, 10, 30)};
  QCOMPARE(extractDates(transactions), expected);
}

void TestFrequency::generate_yearlyRange() {
  Frequency frequency;
  frequency.setPrototype(makePrototype());
  frequency.setFrequency(OpenAccountEnums::Frequency::Yearly);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2024, 2, 29), QDate(2028, 3, 1));

  QCOMPARE(transactions.size(), 5);
  QList<QDate> expected{QDate(2024, 2, 29), QDate(2025, 2, 28),
                        QDate(2026, 2, 28), QDate(2027, 2, 28),
                        QDate(2028, 2, 28)};
  QCOMPARE(extractDates(transactions), expected);
}

void TestFrequency::generate_customRange() {
  Frequency frequency;
  frequency.setPrototype(makePrototype());
  frequency.setFrequency(OpenAccountEnums::Frequency::Custom);
  frequency.setCustomIntervalDays(10);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 1, 1), QDate(2026, 1, 31));

  QCOMPARE(transactions.size(), 4);
  QList<QDate> expected{QDate(2026, 1, 1), QDate(2026, 1, 11),
                        QDate(2026, 1, 21), QDate(2026, 1, 31)};
  QCOMPARE(extractDates(transactions), expected);
}

void TestFrequency::generate_onceCount() {
  Frequency frequency;
  frequency.setPrototype(makePrototype());
  frequency.setFrequency(OpenAccountEnums::Frequency::Once);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 1, 1), 5);

  QCOMPARE(transactions.size(), 1);
  QCOMPARE(transactions.first()->date(), QDate(2026, 1, 1));
}

void TestFrequency::generate_dailyCount() {
  Frequency frequency;
  frequency.setPrototype(makePrototype());
  frequency.setFrequency(OpenAccountEnums::Frequency::Daily);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 1, 1), 3);

  QCOMPARE(transactions.size(), 3);
  QList<QDate> expected{QDate(2026, 1, 1), QDate(2026, 1, 2),
                        QDate(2026, 1, 3)};
  QCOMPARE(extractDates(transactions), expected);
}

void TestFrequency::generate_customCount() {
  Frequency frequency;
  frequency.setPrototype(makePrototype());
  frequency.setFrequency(OpenAccountEnums::Frequency::Custom);
  frequency.setCustomIntervalDays(4);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 1, 1), 4);

  QCOMPARE(transactions.size(), 4);
  QList<QDate> expected{QDate(2026, 1, 1), QDate(2026, 1, 5), QDate(2026, 1, 9),
                        QDate(2026, 1, 13)};
  QCOMPARE(extractDates(transactions), expected);
}

void TestFrequency::generatedTransactions_clonePrototypeData() {
  Frequency frequency;
  const TransactionPtr prototype = makePrototype();
  frequency.setPrototype(prototype);
  frequency.setFrequency(OpenAccountEnums::Frequency::Once);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 5, 12), QDate(2026, 5, 20));

  QCOMPARE(transactions.size(), 1);
  const TransactionPtr generated = transactions.first();

  QCOMPARE(generated->value(), prototype->value());
  QCOMPARE(generated->support(), prototype->support());
  QCOMPARE(generated->movement(), prototype->movement());
  QCOMPARE(generated->isVisible(), prototype->isVisible());
  QCOMPARE(generated->accountId(), prototype->accountId());
  QCOMPARE(generated->category(), prototype->category());
  QCOMPARE(generated->metaData<QString>(QStringLiteral("customMarker")),
           QStringLiteral("prototype"));
}

void TestFrequency::generatedTransactions_overrideDateAndDescription() {
  Frequency frequency;
  const TransactionPtr prototype = makePrototype();
  frequency.setPrototype(prototype);
  frequency.setFrequency(OpenAccountEnums::Frequency::Once);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 5, 12), QDate(2026, 5, 20));

  QCOMPARE(transactions.size(), 1);
  const TransactionPtr generated = transactions.first();

  QCOMPARE(generated->date(), QDate(2026, 5, 12));
  QCOMPARE(generated->description(), QStringLiteral("Rent - 2026-05-12"));
}

void TestFrequency::generatedTransactions_storeFrequencyIdMetadata() {
  Frequency frequency;
  frequency.setPrototype(makePrototype());
  frequency.setFrequency(OpenAccountEnums::Frequency::Daily);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 1, 1), QDate(2026, 1, 2));

  QCOMPARE(transactions.size(), 2);

  for (const TransactionPtr &transaction : transactions) {
    QVERIFY(transaction->hasMetadata(QStringLiteral("frequencyId")));
    QVERIFY(
        !transaction->metaData<QUuid>(QStringLiteral("frequencyId")).isNull());
  }
}

void TestFrequency::generatedTransactions_frequencyIdMatchesSourceId() {
  Frequency frequency;
  const QUuid uid = QUuid::createUuid();
  frequency.setId(uid);
  frequency.setPrototype(makePrototype());
  frequency.setFrequency(OpenAccountEnums::Frequency::Once);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 1, 1), QDate(2026, 1, 10));

  QCOMPARE(transactions.size(), 1);
  QCOMPARE(transactions.first()->metaData<QUuid>(QStringLiteral("frequencyId")),
           uid);
}

void TestFrequency::generatedTransactions_distinctSharedPointers() {
  Frequency frequency;
  frequency.setPrototype(makePrototype());
  frequency.setFrequency(OpenAccountEnums::Frequency::Daily);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 1, 1), 3);

  QCOMPARE(transactions.size(), 3);
  QVERIFY(transactions.at(0) != transactions.at(1));
  QVERIFY(transactions.at(1) != transactions.at(2));
  QVERIFY(transactions.at(0).data() != transactions.at(1).data());
  QVERIFY(transactions.at(1).data() != transactions.at(2).data());
}

void TestFrequency::generatedTransactions_distinctIdsFromPrototype() {
  Frequency frequency;
  const TransactionPtr prototype = makePrototype();
  frequency.setPrototype(prototype);
  frequency.setFrequency(OpenAccountEnums::Frequency::Daily);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 1, 1), 2);

  QCOMPARE(transactions.size(), 2);
  QCOMPARE(transactions.at(0)->id() == prototype->id(), false);
  QCOMPARE(transactions.at(1)->id() == prototype->id(), false);
}

void TestFrequency::generatedTransactions_descriptionWithEmptyPrototype() {
  Frequency frequency;
  TransactionPtr prototype = makePrototype();
  prototype->setDescription(QString());
  frequency.setPrototype(prototype);
  frequency.setFrequency(OpenAccountEnums::Frequency::Once);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 7, 14), QDate(2026, 7, 20));

  QCOMPARE(transactions.size(), 1);
  QCOMPARE(transactions.first()->description(), QStringLiteral("2026-07-14"));
}

void TestFrequency::generatedTransactions_customDateFormat() {
  Frequency frequency;
  frequency.setPrototype(makePrototype());
  frequency.setFrequency(OpenAccountEnums::Frequency::Once);
  frequency.setDateFormat(QStringLiteral("dd/MM/yyyy"));

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 7, 14), QDate(2026, 7, 20));

  QCOMPARE(transactions.size(), 1);
  QCOMPARE(transactions.first()->description(),
           QStringLiteral("Rent - 14/07/2026"));
}

void TestFrequency::generatedTransactions_keepPrototypeUnchanged() {
  Frequency frequency;
  const TransactionPtr prototype = makePrototype();
  const QString originalDescription = prototype->description();
  const QDate originalDate = prototype->date();

  frequency.setPrototype(prototype);
  frequency.setFrequency(OpenAccountEnums::Frequency::Daily);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 1, 1), 2);

  QCOMPARE(transactions.size(), 2);
  QCOMPARE(prototype->description(), originalDescription);
  QCOMPARE(prototype->date(), originalDate);
}

void TestFrequency::generatedTransactions_respectMonthBoundaries() {
  Frequency frequency;
  frequency.setPrototype(makePrototype());
  frequency.setFrequency(OpenAccountEnums::Frequency::Monthly);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2025, 1, 31), 3);

  QCOMPARE(transactions.size(), 3);
  QList<QDate> expected{QDate(2025, 1, 31), QDate(2025, 2, 28),
                        QDate(2025, 3, 28)};
  QCOMPARE(extractDates(transactions), expected);
}

void TestFrequency::sharedFromThis_returnsSameInstance() {
  const FrequencyPtr frequency = FrequencyPtr::create();
  const FrequencyPtr same = frequency->sharedFromThis();

  QCOMPARE(same.data(), frequency.data());
}

void TestFrequency::generatedTransactions_haveDistinctIds() {
  Frequency frequency;
  const TransactionPtr prototype = makePrototype();
  frequency.setPrototype(prototype);
  frequency.setFrequency(OpenAccountEnums::Frequency::Daily);

  const QList<TransactionPtr> transactions =
      frequency.generate(QDate(2026, 1, 1), 3);

  QCOMPARE(transactions.size(), 3);

  for (const TransactionPtr &t : transactions) {
    QVERIFY(!t->id().isNull());
    QVERIFY(t->id() != prototype->id());
  }

  QVERIFY(transactions.at(0)->id() != transactions.at(1)->id());
  QVERIFY(transactions.at(1)->id() != transactions.at(2)->id());
}

QTEST_MAIN(TestFrequency)
#include "tst_frequency.moc"
