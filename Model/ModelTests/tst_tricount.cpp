#include "../include/Model/tricount.h"
#include "../include/Model/tricountexpense.h"
#include "../include/Model/tricountmember.h"
#include <QSignalSpy>
#include <QtTest>

/*
 * Unit tests for Tricount
 * Tests membership management, balance computation, settlement minimisation,
 * transaction generation linked to profile accounts, JSON round-trip, signals.
 */

class TestTricount : public QObject {
    Q_OBJECT

private slots:
    void testSetGet();
    void testAddRemoveMember();
    void testAddRemoveExpense();
    void testMemberLookup();
    void testBalancesSimple();
    void testBalancesSumToZero();
    void testBalancesWithExclusion();
    void testSettlementsZeroOutBalances();
    void testGenerateTransactionsPayerAndTransfers();
    void testGenerateTransactionsSkipsGuests();
    void testGenerateTransactionsTaggedWithTricountId();
    void testJsonRoundTrip();
    void testSignal_membersChanged();

private:
    static TricountMemberPtr makeMember(const QString& name, const QUuid& account = {}) {
        auto m = TricountMemberPtr::create();
        m->setId(QUuid::createUuid());
        m->setName(name);
        if (!account.isNull()) {
            m->setProfileId(QUuid::createUuid());
            m->setAccountId(account);
        }
        return m;
    }
    static TricountExpensePtr makeExpense(const QUuid& payer, double amount,
                                          const QList<QUuid>& excluded = {}) {
        auto e = TricountExpensePtr::create();
        e->setId(QUuid::createUuid());
        auto tx = TransactionPtr::create();
        tx->setValue(amount);
        tx->setMovement(OpenAccountEnums::Movement::Debit);
        e->setTransaction(tx);
        e->setPayerId(payer);
        e->setExcluded(excluded);
        return e;
    }
    static double balanceSum(const QMap<QUuid, double>& b) {
        double s = 0.0;
        for (double v : b) s += v;
        return s;
    }
};

void TestTricount::testSetGet() {
    Tricount t;
    const QUuid id = QUuid::createUuid();
    const QDate date(2026, 7, 7);

    t.setId(id);
    t.setName("Vacances");
    t.setDescription("Été 2026");
    t.setDate(date);
    t.setCurrency("EUR");

    QCOMPARE(t.id(), id);
    QCOMPARE(t.name(), QString("Vacances"));
    QCOMPARE(t.description(), QString("Été 2026"));
    QCOMPARE(t.date(), date);
    QCOMPARE(t.currency(), QString("EUR"));
}

void TestTricount::testAddRemoveMember() {
    Tricount t;
    auto a = makeMember("A");
    auto b = makeMember("B");

    t.addMember(a);
    t.addMember(b);
    QCOMPARE(t.members().size(), 2);

    t.removeMember(a->id());
    QCOMPARE(t.members().size(), 1);
    QCOMPARE(t.members().first()->id(), b->id());
}

void TestTricount::testAddRemoveExpense() {
    Tricount t;
    auto a = makeMember("A");
    t.addMember(a);
    auto e = makeExpense(a->id(), 10.0);

    t.addExpense(e);
    QCOMPARE(t.expenses().size(), 1);

    t.removeExpense(e->id());
    QCOMPARE(t.expenses().size(), 0);
}

void TestTricount::testMemberLookup() {
    Tricount t;
    auto a = makeMember("A");
    t.addMember(a);

    QCOMPARE(t.member(a->id())->id(), a->id());
    QVERIFY(t.member(QUuid::createUuid()).isNull());
}

void TestTricount::testBalancesSimple() {
    Tricount t;
    auto a = makeMember("A");
    auto b = makeMember("B");
    auto c = makeMember("C");
    t.addMember(a);
    t.addMember(b);
    t.addMember(c);
    t.addExpense(makeExpense(a->id(), 30.0)); // A pays 30, split 3 ways

    const auto bal = t.balances();
    QCOMPARE(bal.value(a->id()), 20.0);
    QCOMPARE(bal.value(b->id()), -10.0);
    QCOMPARE(bal.value(c->id()), -10.0);
}

void TestTricount::testBalancesSumToZero() {
    Tricount t;
    auto a = makeMember("A");
    auto b = makeMember("B");
    auto c = makeMember("C");
    t.addMember(a);
    t.addMember(b);
    t.addMember(c);
    t.addExpense(makeExpense(a->id(), 30.0));
    t.addExpense(makeExpense(b->id(), 10.0));
    t.addExpense(makeExpense(c->id(), 7.0));

    QVERIFY(qAbs(balanceSum(t.balances())) < 1e-9);
}

void TestTricount::testBalancesWithExclusion() {
    Tricount t;
    auto a = makeMember("A");
    auto b = makeMember("B");
    auto c = makeMember("C");
    t.addMember(a);
    t.addMember(b);
    t.addMember(c);
    // A pays 20, C excluded => split between A and B only.
    t.addExpense(makeExpense(a->id(), 20.0, {c->id()}));

    const auto bal = t.balances();
    QCOMPARE(bal.value(a->id()), 10.0);
    QCOMPARE(bal.value(b->id()), -10.0);
    QCOMPARE(bal.value(c->id()), 0.0);
}

void TestTricount::testSettlementsZeroOutBalances() {
    Tricount t;
    auto a = makeMember("A");
    auto b = makeMember("B");
    auto c = makeMember("C");
    t.addMember(a);
    t.addMember(b);
    t.addMember(c);
    t.addExpense(makeExpense(a->id(), 30.0));
    t.addExpense(makeExpense(b->id(), 12.0));

    auto bal = t.balances();
    const auto settlements = t.settlements();
    QVERIFY(!settlements.isEmpty());

    // Applying every settlement must drive all balances to zero.
    for (const auto& s : settlements) {
        QVERIFY(s.amount > 0.0);
        bal[s.from] += s.amount;
        bal[s.to] -= s.amount;
    }
    for (double v : bal)
        QVERIFY(qAbs(v) < 1e-9);
}

void TestTricount::testGenerateTransactionsPayerAndTransfers() {
    const QUuid accA = QUuid::createUuid();
    const QUuid accB = QUuid::createUuid();

    Tricount t;
    auto a = makeMember("A", accA);
    auto b = makeMember("B", accB);
    t.addMember(a);
    t.addMember(b);
    t.addExpense(makeExpense(a->id(), 20.0)); // A pays 20 => B owes 10

    const auto txs = t.generateTransactions();

    bool payerDebit = false, transferDebit = false, transferCredit = false;
    for (const auto& tx : txs) {
        if (tx->accountId() == accA &&
            tx->movement() == OpenAccountEnums::Movement::Debit &&
            qFuzzyCompare(tx->value() + 1.0, 20.0 + 1.0))
            payerDebit = true;
        if (tx->accountId() == accB &&
            tx->movement() == OpenAccountEnums::Movement::Debit &&
            qFuzzyCompare(tx->value() + 1.0, 10.0 + 1.0))
            transferDebit = true;
        if (tx->accountId() == accA &&
            tx->movement() == OpenAccountEnums::Movement::Credit &&
            qFuzzyCompare(tx->value() + 1.0, 10.0 + 1.0))
            transferCredit = true;
    }
    QVERIFY(payerDebit);
    QVERIFY(transferDebit);
    QVERIFY(transferCredit);
}

void TestTricount::testGenerateTransactionsSkipsGuests() {
    Tricount t;
    auto guest = makeMember("Guest");          // no account
    auto linked = makeMember("Linked", QUuid::createUuid());
    t.addMember(guest);
    t.addMember(linked);
    t.addExpense(makeExpense(guest->id(), 20.0)); // guest pays => no payer tx

    const auto txs = t.generateTransactions();
    for (const auto& tx : txs)
        QVERIFY(!tx->accountId().isNull());
}

void TestTricount::testGenerateTransactionsTaggedWithTricountId() {
    Tricount t;
    t.setId(QUuid::createUuid());
    auto a = makeMember("A", QUuid::createUuid());
    auto b = makeMember("B", QUuid::createUuid());
    t.addMember(a);
    t.addMember(b);
    t.addExpense(makeExpense(a->id(), 20.0));

    const auto txs = t.generateTransactions();
    QVERIFY(!txs.isEmpty());
    for (const auto& tx : txs)
        QCOMPARE(tx->metaData<QUuid>("tricountId"), t.id());
}

void TestTricount::testJsonRoundTrip() {
    Tricount t;
    t.setId(QUuid::createUuid());
    t.setName("Weekend");
    t.setCurrency("EUR");
    auto a = makeMember("A", QUuid::createUuid());
    auto b = makeMember("B");
    t.addMember(a);
    t.addMember(b);
    t.addExpense(makeExpense(a->id(), 42.0, {b->id()}));

    const QJsonObject obj = t.toJson();
    Tricount copy(obj);

    QCOMPARE(copy.id(), t.id());
    QCOMPARE(copy.name(), t.name());
    QCOMPARE(copy.currency(), t.currency());
    QCOMPARE(copy.members().size(), 2);
    QCOMPARE(copy.expenses().size(), 1);
    QCOMPARE(copy.expenses().first()->amount(), 42.0);
    QCOMPARE(copy.expenses().first()->excluded(), QList<QUuid>({b->id()}));
    // A pays 42 and B is excluded, so only A shares: A bears its own expense.
    // Balances must be reproducible after a round-trip and net to zero here.
    QCOMPARE(copy.balances().value(a->id()), 0.0);
    QCOMPARE(copy.balances().value(b->id()), 0.0);
}

void TestTricount::testSignal_membersChanged() {
    Tricount t;
    QSignalSpy spy(&t, SIGNAL(membersChanged()));
    t.addMember(makeMember("A"));
    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestTricount)
#include "tst_tricount.moc"
