#include "Model/Business/tricount.h"

#include <QJsonArray>
#include <QtGlobal>
#include <algorithm>

// --- TricountSettlement -----------------------------------------------------

bool TricountSettlement::operator==(const TricountSettlement& other) const {
    return from == other.from && to == other.to &&
           qFuzzyCompare(amount + 1.0, other.amount + 1.0);
}

// --- Tricount ---------------------------------------------------------------

Tricount::Tricount(QObject* parent) : QObject(parent), MetaData() {
    qRegisterMetaType<TricountMemberPtr>("TricountMemberPtr");
    qRegisterMetaType<TricountExpensePtr>("TricountExpensePtr");
    qRegisterMetaType<QList<TricountMemberPtr>>("QList<TricountMemberPtr>");
    qRegisterMetaType<QList<TricountExpensePtr>>("QList<TricountExpensePtr>");
    qRegisterMetaType<TricountSettlement>("TricountSettlement");
    qRegisterMetaType<QList<TricountSettlement>>("QList<TricountSettlement>");
    setMetadata(Key::id, QUuid::createUuid());
    setMetadata(Key::name, QString{});
    setMetadata(Key::description, QString{});
    setMetadata(Key::date, QDate{});
    setMetadata(Key::currency, QString{});
}

Tricount::Tricount(const QJsonObject& json, QObject* parent)
    : QObject(parent), MetaData() {
    qRegisterMetaType<TricountMemberPtr>("TricountMemberPtr");
    qRegisterMetaType<TricountExpensePtr>("TricountExpensePtr");
    qRegisterMetaType<QList<TricountMemberPtr>>("QList<TricountMemberPtr>");
    qRegisterMetaType<QList<TricountExpensePtr>>("QList<TricountExpensePtr>");
    qRegisterMetaType<TricountSettlement>("TricountSettlement");
    qRegisterMetaType<QList<TricountSettlement>>("QList<TricountSettlement>");
    setMetadata(Key::id, QUuid{});
    setMetadata(Key::name, QString{});
    setMetadata(Key::description, QString{});
    setMetadata(Key::date, QDate{});
    setMetadata(Key::currency, QString{});
    Tricount::fromJson(json);
}

// --- Lookup ----------------------------------------------------------------

TricountMemberPtr Tricount::member(QUuid id) const {
    for (const TricountMemberPtr& m : m_members)
        if (m && m->id() == id)
            return m;
    return {};
}

// --- Balances / settlements -------------------------------------------------

QMap<QUuid, double> Tricount::balances() const {
    QMap<QUuid, double> balance;

    QList<QUuid> memberIds;
    memberIds.reserve(m_members.size());
    for (const TricountMemberPtr& m : m_members)
        if (m) {
            memberIds.append(m->id());
            balance.insert(m->id(), 0.0);
        }

    for (const TricountExpensePtr& e : m_expenses) {
        if (!e || !e->isValid())
            continue;

        balance[e->payerId()] += e->amount();

        const QMap<QUuid, double> shares = e->shareByMember(memberIds);
        for (auto it = shares.cbegin(); it != shares.cend(); ++it)
            balance[it.key()] -= it.value();
    }

    return balance;
}

QList<TricountSettlement> Tricount::settlements() const {
    const QMap<QUuid, double> balance = balances();

    // Work in integer cents so repeated subtraction never drifts.
    QList<QPair<QUuid, qint64>> creditors; // owed money (positive)
    QList<QPair<QUuid, qint64>> debtors;   // owe money (stored positive)
    for (auto it = balance.cbegin(); it != balance.cend(); ++it) {
        const qint64 cents = qRound64(it.value() * 100.0);
        if (cents > 0)
            creditors.append({it.key(), cents});
        else if (cents < 0)
            debtors.append({it.key(), -cents});
    }

    const auto byAmountThenId = [](const QPair<QUuid, qint64>& a,
                                   const QPair<QUuid, qint64>& b) {
        if (a.second != b.second)
            return a.second > b.second;
        return a.first < b.first;
    };
    std::sort(creditors.begin(), creditors.end(), byAmountThenId);
    std::sort(debtors.begin(), debtors.end(), byAmountThenId);

    QList<TricountSettlement> result;
    int i = 0, j = 0;
    while (i < debtors.size() && j < creditors.size()) {
        const qint64 pay = qMin(debtors[i].second, creditors[j].second);
        if (pay > 0) {
            TricountSettlement s;
            s.from = debtors[i].first;
            s.to = creditors[j].first;
            s.amount = static_cast<double>(pay) / 100.0;
            result.append(s);
        }
        debtors[i].second -= pay;
        creditors[j].second -= pay;
        if (debtors[i].second == 0)
            ++i;
        if (creditors[j].second == 0)
            ++j;
    }

    return result;
}

// --- Transaction generation -------------------------------------------------

QList<TransactionPtr> Tricount::generateTransactions() const {
    QList<TransactionPtr> result;

    // 1) Real cash outflow of each profile-linked payer.
    for (const TricountExpensePtr& e : m_expenses) {
        if (!e || !e->isValid())
            continue;

        const TricountMemberPtr payer = member(e->payerId());
        if (!payer || payer->accountId().isNull())
            continue;

        const TransactionPtr src = e->transaction();
        TransactionPtr tx = TransactionPtr::create();
        tx->setId(QUuid::createUuid());
        tx->setValue(e->amount());
        tx->setMovement(OpenAccountEnums::Movement::Debit);
        tx->setAccountId(payer->accountId());
        if (src) {
            tx->setDescription(src->description());
            tx->setSupport(src->support());
            tx->setDate(src->date());
            tx->setCategory(src->category());
        }
        tx->setMetadata(Key::tricountId, id());
        tx->setMetadata(QStringLiteral("expenseId"), e->id());
        result.append(tx);
    }

    // 2) Reimbursement transfers between members that carry an account.
    const QList<TricountSettlement> transfers = settlements();
    for (const TricountSettlement& s : transfers) {
        const TricountMemberPtr from = member(s.from);
        const TricountMemberPtr to = member(s.to);

        if (from && !from->accountId().isNull()) {
            TransactionPtr tx = TransactionPtr::create();
            tx->setId(QUuid::createUuid());
            tx->setValue(s.amount);
            tx->setMovement(OpenAccountEnums::Movement::Debit);
            tx->setSupport(OpenAccountEnums::Support::Transfer);
            tx->setAccountId(from->accountId());
            tx->setMetadata(Key::tricountId, id());
            result.append(tx);
        }
        if (to && !to->accountId().isNull()) {
            TransactionPtr tx = TransactionPtr::create();
            tx->setId(QUuid::createUuid());
            tx->setValue(s.amount);
            tx->setMovement(OpenAccountEnums::Movement::Credit);
            tx->setSupport(OpenAccountEnums::Support::Transfer);
            tx->setAccountId(to->accountId());
            tx->setMetadata(Key::tricountId, id());
            result.append(tx);
        }
    }

    return result;
}

// --- Setters ---------------------------------------------------------------

void Tricount::setId(QUuid v) {
    if (id() == v) return;
    setMetadata(Key::id, v);
    emit idChanged();
    emit changed();
}

void Tricount::setName(QString v) {
    if (name() == v) return;
    setMetadata(Key::name, v);
    emit nameChanged();
    emit changed();
}

void Tricount::setDescription(QString v) {
    if (description() == v) return;
    setMetadata(Key::description, v);
    emit descriptionChanged();
    emit changed();
}

void Tricount::setDate(QDate v) {
    if (date() == v) return;
    setMetadata(Key::date, v);
    emit dateChanged();
    emit changed();
}

void Tricount::setCurrency(QString v) {
    if (currency() == v) return;
    setMetadata(Key::currency, v);
    emit currencyChanged();
    emit changed();
}

void Tricount::setMembers(QList<TricountMemberPtr> v) {
    m_members = v;
    emit membersChanged();
    emit changed();
}

void Tricount::setExpenses(QList<TricountExpensePtr> v) {
    m_expenses = v;
    emit expensesChanged();
    emit changed();
}

void Tricount::addMember(TricountMemberPtr member) {
    if (member.isNull())
        return;
    m_members.append(member);
    emit membersChanged();
    emit changed();
}

void Tricount::removeMember(QUuid id) {
    const auto before = m_members.size();
    m_members.removeIf(
        [&](const TricountMemberPtr& m) { return m && m->id() == id; });
    if (m_members.size() != before) {
        emit membersChanged();
        emit changed();
    }
}

void Tricount::addExpense(TricountExpensePtr expense) {
    if (expense.isNull())
        return;
    m_expenses.append(expense);
    emit expensesChanged();
    emit changed();
}

void Tricount::removeExpense(QUuid id) {
    const auto before = m_expenses.size();
    m_expenses.removeIf(
        [&](const TricountExpensePtr& e) { return e && e->id() == id; });
    if (m_expenses.size() != before) {
        emit expensesChanged();
        emit changed();
    }
}

// --- JSON ------------------------------------------------------------------

QJsonObject Tricount::toJson() const {
    // Qualified call to the base serializer: an unqualified toJson() would
    // re-dispatch to this override and recurse infinitely. MetaData already
    // knows how to serialize id/name/description/date/currency; members and
    // expenses are QLists of nested objects living outside the MetaData map
    // (see m_members/m_expenses), so they keep their dedicated encoding.
    QJsonObject o = MetaData::toJson();

    QJsonArray members;
    for (const TricountMemberPtr& m : m_members)
        if (m)
            members.append(m->toJson());
    o.insert(Key::members, members);

    QJsonArray expenses;
    for (const TricountExpensePtr& e : m_expenses)
        if (e)
            expenses.append(e->toJson());
    o.insert(Key::expenses, expenses);

    return o;
}

void Tricount::fromJson(const QJsonObject& json) {
    // MetaData::fromJson() restores id/name/description/date/currency;
    // members/expenses are patched afterwards via their setters since they
    // live outside the MetaData map.
    MetaData::fromJson(json);

    QList<TricountMemberPtr> members;
    for (const QJsonValue& v : json.value(Key::members).toArray())
        members.append(TricountMemberPtr::create(v.toObject()));
    setMembers(members);

    QList<TricountExpensePtr> expenses;
    for (const QJsonValue& v : json.value(Key::expenses).toArray())
        expenses.append(TricountExpensePtr::create(v.toObject()));
    setExpenses(expenses);
}
