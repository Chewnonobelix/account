#include <QJsonObject>
#include <QSignalSpy>
#include <QUuid>
#include <QtTest>

#include "../include/Model/Business/category.h"
#include "../include/Model/enums.h"

class TestCategory : public QObject {
  Q_OBJECT

private slots:
  void testDefaultConstruction();
  void testIdProperty();
  void testDirectionProperty();
  void testNameProperty();
  void testToJsonRoundTrip();
  void testJsonConstructorWithPartialData();
};

void TestCategory::testDefaultConstruction() {
  Category category;

  // Default id should be null
  QCOMPARE(category.id().isNull(), true);

  // Default name should be empty
  QCOMPARE(category.name(), QString());

  // Direction should be a valid Movement value; we assume default constructed
  // value is valid
  OpenAccountEnums::Movement direction = category.direction();
  QVERIFY(direction == OpenAccountEnums::Movement::Credit ||
          direction == OpenAccountEnums::Movement::Debit ||
          direction == OpenAccountEnums::Movement::Both);
}

void TestCategory::testIdProperty() {
  Category category;
  QSignalSpy spy(&category, &Category::idChanged);
  QVERIFY(spy.isValid());

  const QUuid firstId = QUuid::createUuid();
  const QUuid secondId = QUuid::createUuid();

  category.setId(firstId);
  QCOMPARE(category.id(), firstId);
  QCOMPARE(spy.count(), 1);

  // Setting the same value should not emit the signal again
  category.setId(firstId);
  QCOMPARE(category.id(), firstId);
  QCOMPARE(spy.count(), 1);

  // Changing the value should emit the signal
  category.setId(secondId);
  QCOMPARE(category.id(), secondId);
  QCOMPARE(spy.count(), 2);
}

void TestCategory::testDirectionProperty() {
  Category category;
  QSignalSpy spy(&category, &Category::directionChanged);
  QVERIFY(spy.isValid());

  // Use a value different from the expected default (Credit) to ensure a change
  category.setDirection(OpenAccountEnums::Movement::Debit);
  QCOMPARE(category.direction(), OpenAccountEnums::Movement::Debit);
  QCOMPARE(spy.count(), 1);

  // Setting the same value should not emit the signal again
  category.setDirection(OpenAccountEnums::Movement::Debit);
  QCOMPARE(category.direction(), OpenAccountEnums::Movement::Debit);
  QCOMPARE(spy.count(), 1);

  // Switch back to Credit
  category.setDirection(OpenAccountEnums::Movement::Credit);
  QCOMPARE(category.direction(), OpenAccountEnums::Movement::Credit);
  QCOMPARE(spy.count(), 2);
}

void TestCategory::testNameProperty() {
  Category category;
  QSignalSpy spy(&category, &Category::nameChanged);
  QVERIFY(spy.isValid());

  const QString firstName = QStringLiteral("Food");
  const QString secondName = QStringLiteral("Transport");

  category.setName(firstName);
  QCOMPARE(category.name(), firstName);
  QCOMPARE(spy.count(), 1);

  // Setting the same value should not emit the signal again
  category.setName(firstName);
  QCOMPARE(category.name(), firstName);
  QCOMPARE(spy.count(), 1);

  // Changing the value should emit the signal
  category.setName(secondName);
  QCOMPARE(category.name(), secondName);
  QCOMPARE(spy.count(), 2);
}

void TestCategory::testToJsonRoundTrip() {
  Category category;

  const QUuid id = QUuid::createUuid();
  const QString name = QStringLiteral("Groceries");
  const OpenAccountEnums::Movement movement = OpenAccountEnums::Movement::Debit;

  category.setId(id);
  category.setName(name);
  category.setDirection(movement);

  const QJsonObject json = category.toJson();

  // Basic JSON structure checks
  QVERIFY(json.contains("id"));
  QVERIFY(json.contains("name"));
  QVERIFY(json.contains("direction"));

  QCOMPARE(json.value("name").toString(), name);
  QCOMPARE(json.value("id").toString(), id.toString(QUuid::WithoutBraces));
  QVERIFY(json.value("direction").isString());

  // Reconstruct from JSON and compare
  Category other(json);

  QCOMPARE(other.id(), id);
  QCOMPARE(other.name(), name);
  QCOMPARE(other.direction(), movement);
}

void TestCategory::testJsonConstructorWithPartialData() {
  QJsonObject json;

  const QUuid id = QUuid::createUuid();
  json.insert(QStringLiteral("id"), id.toString(QUuid::WithoutBraces));
  // No name, no direction in this JSON

  Category category(json);

  // Id should be loaded
  QCOMPARE(category.id(), id);

  // Name was not provided -> should be default (empty string)
  QCOMPARE(category.name(), QString());

  // Direction not provided -> should be a valid Movement default
  OpenAccountEnums::Movement direction = category.direction();
  QVERIFY(direction == OpenAccountEnums::Movement::Credit ||
          direction == OpenAccountEnums::Movement::Debit ||
          direction == OpenAccountEnums::Movement::Both);
}

QTEST_MAIN(TestCategory)
#include "tst_category.moc"
