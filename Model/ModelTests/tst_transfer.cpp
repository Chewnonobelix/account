#include "../include/Model/enums.h"
#include "../include/Model/Business/transaction.h"
#include "../include/Model/Business/transfer.h"

#include <QSignalSpy>
#include <QtTest/QtTest>

class TestTransfer : public QObject {
  Q_OBJECT

private slots:
  void defaults_are_sane() {
    Transfer transfer;

    QVERIFY(transfer.debitAccountId().isNull());
    QVERIFY(transfer.creditAccountId().isNull());
    QCOMPARE(transfer.amount(), 0.0);
    QVERIFY(transfer.date().isValid());
    QCOMPARE(transfer.description(), QString());
    QVERIFY(!transfer.isValid());
  }

  void setters_apply_guards() {
    Transfer transfer;

    transfer.setAmount(-50.0);
    QCOMPARE(transfer.amount(), 0.0);

    transfer.setAmount(120.5);
    QCOMPARE(transfer.amount(), 120.5);
  }

  void isValid_requiresDistinctAccountsPositiveAmountAndDate() {
    Transfer transfer;
    const QUuid debitAccount = QUuid::createUuid();
    const QUuid creditAccount = QUuid::createUuid();

    QVERIFY(!transfer.isValid()); // no accounts, no amount

    transfer.setDebitAccountId(debitAccount);
    transfer.setCreditAccountId(creditAccount);
    QVERIFY(!transfer.isValid()); // amount still 0

    transfer.setAmount(100.0);
    QVERIFY(transfer.isValid());

    transfer.setCreditAccountId(debitAccount); // same account both sides
    QVERIFY(!transfer.isValid());

    transfer.setCreditAccountId(creditAccount);
    transfer.setDate(QDate());
    QVERIFY(!transfer.isValid());
  }

  void signals_fire_once_per_effective_change() {
    Transfer transfer;
    QSignalSpy changedSpy(&transfer, &Transfer::changed);
    QSignalSpy amountSpy(&transfer, &Transfer::amountChanged);
    QSignalSpy debitSpy(&transfer, &Transfer::debitAccountIdChanged);

    transfer.setAmount(100.0);
    transfer.setAmount(100.0); // no-op
    QCOMPARE(amountSpy.count(), 1);

    const QUuid debitAccount = QUuid::createUuid();
    transfer.setDebitAccountId(debitAccount);
    transfer.setDebitAccountId(debitAccount); // no-op
    QCOMPARE(debitSpy.count(), 1);

    QCOMPARE(changedSpy.count(), 2);
  }

  void generateTransactions_buildsDebitAndCreditLegs() {
    Transfer transfer;
    const QUuid debitAccount = QUuid::createUuid();
    const QUuid creditAccount = QUuid::createUuid();

    transfer.setDebitAccountId(debitAccount);
    transfer.setCreditAccountId(creditAccount);
    transfer.setAmount(300.0);
    transfer.setDate(QDate(2026, 5, 1));
    transfer.setDescription(QStringLiteral("Virement vers livret"));

    const QList<TransactionPtr> legs = transfer.generateTransactions();

    QCOMPARE(legs.size(), 2);

    const TransactionPtr debitLeg = legs.at(0);
    QCOMPARE(debitLeg->movement(), OpenAccountEnums::Movement::Debit);
    QCOMPARE(debitLeg->accountId(), debitAccount);
    QCOMPARE(debitLeg->value(), 300.0);
    QCOMPARE(debitLeg->date(), QDate(2026, 5, 1));
    QCOMPARE(debitLeg->description(), QStringLiteral("Virement vers livret"));
    QCOMPARE(debitLeg->support(), OpenAccountEnums::Support::Transfer);
    QVERIFY(debitLeg->isVisible());

    const TransactionPtr creditLeg = legs.at(1);
    QCOMPARE(creditLeg->movement(), OpenAccountEnums::Movement::Credit);
    QCOMPARE(creditLeg->accountId(), creditAccount);
    QCOMPARE(creditLeg->value(), 300.0);
    QCOMPARE(creditLeg->date(), QDate(2026, 5, 1));
    QCOMPARE(creditLeg->description(), QStringLiteral("Virement vers livret"));
    QCOMPARE(creditLeg->support(), OpenAccountEnums::Support::Transfer);
    QVERIFY(creditLeg->isVisible());
  }

  void generateTransactions_invalidTransferReturnsEmpty() {
    Transfer transfer;

    QCOMPARE(transfer.generateTransactions().size(), 0);

    transfer.setDebitAccountId(QUuid::createUuid());
    transfer.setCreditAccountId(transfer.debitAccountId()); // same account
    transfer.setAmount(50.0);

    QCOMPARE(transfer.generateTransactions().size(), 0);
  }
};

QTEST_MAIN(TestTransfer)
#include "tst_transfer.moc"
