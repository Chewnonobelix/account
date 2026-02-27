#include "../include/Model/enums.h"
#include "../include/Model/total.h"
#include "../include/Model/transaction.h"
#include <QSharedPointer>
#include <QSignalSpy>
#include <QtTest>

class tst_Transaction : public QObject {
  Q_OBJECT
private slots:
  // ---------- Basics ----------
  void defaults_are_sane() {
    Transaction t;
    QVERIFY(!t.id().isNull());
    QCOMPARE(t.value(), 0.0);
    QVERIFY(t.isVisible());
    QVERIFY(t.date().isValid());
    // sharedFromThis() must be null when not owned by QSharedPointer
    QVERIFY(t.sharedFromThis().isNull());
  }

  void shared_from_this_when_shared() {
    auto sp = QSharedPointer<Transaction>::create();
    auto s2 = sp->sharedFromThis();
    QVERIFY(!s2.isNull());
    QCOMPARE(s2.data(), sp.data());
  }

  // ---------- Signals: one test per property ----------
  void id_changed_signal() {
    Transaction t;
    QSignalSpy spy(&t, &Transaction::idChanged);
    auto v = QUuid::createUuid();
    t.setId(v);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(t.id(), v);
    t.setId(v); // no change
    QCOMPARE(spy.count(), 1);
  }

  void value_changed_signal() {
    Transaction t;
    QSignalSpy spy(&t, &Transaction::valueChanged);
    t.setValue(123.45);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(t.value(), 123.45);
    t.setValue(123.45); // no change
    QCOMPARE(spy.count(), 1);
  }

  void description_changed_signal() {
    Transaction t;
    QSignalSpy spy(&t, &Transaction::descriptionChanged);
    t.setDescription("Hello");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(t.description(), QString("Hello"));
    t.setDescription("Hello");
    QCOMPARE(spy.count(), 1);
  }

  void support_changed_signal() {
    Transaction t;
    QSignalSpy spy(&t, &Transaction::supportChanged);
    t.setSupport(OpenAccountEnums::Support::Card);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(t.support(), OpenAccountEnums::Support::Card);
    t.setSupport(OpenAccountEnums::Support::Card);
    QCOMPARE(spy.count(), 1);
  }

  void date_changed_signal() {
    Transaction t;
    QSignalSpy spy(&t, &Transaction::dateChanged);
    QDate d(2025, 1, 31);
    t.setDate(d);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(t.date(), d);
    t.setDate(d);
    QCOMPARE(spy.count(), 1);
  }

  void movement_changed_signal() {
    Transaction t;
    QSignalSpy spy(&t, &Transaction::movementChanged);
    t.setMovement(OpenAccountEnums::Movement::Debit);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(t.movement(), OpenAccountEnums::Movement::Debit);
    t.setMovement(OpenAccountEnums::Movement::Debit);
    QCOMPARE(spy.count(), 1);
  }

  void isVisible_changed_signal() {
    Transaction t;
    QSignalSpy spy(&t, &Transaction::isVisibleChanged);
    t.setIsVisible(false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(t.isVisible(), false);
    t.setIsVisible(false);
    QCOMPARE(spy.count(), 1);
  }

  void accountId_changed_signal() {
    Transaction t;
    QSignalSpy spy(&t, &Transaction::accountIdChanged);
    QUuid aid = QUuid::createUuid();
    t.setAccountId(aid);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(t.accountId(), aid);
    t.setAccountId(aid);
    QCOMPARE(spy.count(), 1);
  }

  void category_changed_signal() {
    Transaction t;
    auto cat = QUuid::createUuid();
    QSignalSpy spy(&t, &Transaction::categoryChanged);
    t.setCategory(cat);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(t.category(), cat);
    t.setCategory(cat);
    QCOMPARE(spy.count(), 1);
  }

  // ---------- JSON round-trip ----------
  void json_roundtrip() {
    Transaction t;
    const auto id = QUuid::createUuid();
    auto cat = QUuid::createUuid();
    t.setId(id);
    t.setValue(42.0);
    t.setDescription("Test");
    t.setSupport(OpenAccountEnums::Support::Cash);
    t.setDate(QDate(2025, 8, 18));
    t.setMovement(OpenAccountEnums::Movement::Debit);
    t.setIsVisible(false);
    t.setAccountId(QUuid::createUuid());
    t.setCategory(cat);

    const QJsonObject j = t.toJson();
    Transaction r(j);

    // id is restored from JSON (Transaction ctor sets a uuid first, then
    // overrides if present)
    QCOMPARE(r.id(), id);
    QCOMPARE(r.value(), t.value());
    QCOMPARE(r.description(), t.description());
    QCOMPARE(r.support(), t.support());
    QCOMPARE(r.date(), t.date());
    QCOMPARE(r.movement(), t.movement());
    QCOMPARE(r.isVisible(), t.isVisible());
    QCOMPARE(r.accountId(), t.accountId());
    QCOMPARE(r.category(), t.category());
  }

  // ---------- Operators (+) ----------
  void plus_tx_tx_builds_total() {
    auto t1 = QSharedPointer<Transaction>::create();
    t1->setValue(100.0);
    t1->setMovement(OpenAccountEnums::Movement::Credit);
    t1->setDate(QDate(2025, 1, 1));

    auto t2 = QSharedPointer<Transaction>::create();
    t2->setValue(40.0);
    t2->setMovement(OpenAccountEnums::Movement::Debit);
    t2->setDate(QDate(2025, 1, 5));

    // Operators accept references; sharedFromThis() is valid because objects
    // are owned by QSharedPointer
    Total T = *t1 + *t2;

    QCOMPARE(T.evaluate(), 60.0);
    QCOMPARE(T.from(), QDate(2025, 1, 1));
    QCOMPARE(T.to(), QDate(2025, 1, 5));
  }

  void plus_tx_total_merges() {
    auto t = QSharedPointer<Transaction>::create();
    t->setValue(10.0);
    t->setMovement(OpenAccountEnums::Movement::Credit);
    t->setDate(QDate(2025, 2, 2));

    Total A;          // empty
    Total B = *t + A; // Transaction + Total

    QCOMPARE(B.evaluate(), 10.0);
    QCOMPARE(B.from(), QDate(2025, 2, 2));
    QCOMPARE(B.to(), QDate(2025, 2, 2));
  }

  void plus_total_tx_merges() {
    auto t = QSharedPointer<Transaction>::create();
    t->setValue(5.0);
    t->setMovement(OpenAccountEnums::Movement::Debit);
    t->setDate(QDate(2025, 2, 3));

    Total A;          // empty
    Total C = A + *t; // Total + Transaction

    QCOMPARE(C.evaluate(), -5.0);
    QCOMPARE(C.from(), QDate(2025, 2, 3));
    QCOMPARE(C.to(), QDate(2025, 2, 3));
  }

  void plus_chain_accumulates_and_bounds() {
    auto t1 = QSharedPointer<Transaction>::create();
    t1->setValue(10.0);
    t1->setMovement(OpenAccountEnums::Movement::Credit);
    t1->setDate(QDate(2025, 3, 1));
    auto t2 = QSharedPointer<Transaction>::create();
    t2->setValue(3.0);
    t2->setMovement(OpenAccountEnums::Movement::Debit);
    t2->setDate(QDate(2025, 3, 4));
    auto t3 = QSharedPointer<Transaction>::create();
    t3->setValue(20.0);
    t3->setMovement(OpenAccountEnums::Movement::Credit);
    t3->setDate(QDate(2025, 2, 25));

    Total T = *t1 + *t2; // Total from two
    T = T + *t3;         // merge third

    QCOMPARE(T.evaluate(), (10.0 - 3.0 + 20.0));
    QCOMPARE(T.from(), QDate(2025, 2, 25)); // earliest date
    QCOMPARE(T.to(), QDate(2025, 3, 4));    // latest date
  }
};

#include "tst_transaction.moc"
