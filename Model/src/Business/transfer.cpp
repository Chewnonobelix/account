#include "Model/Business/transfer.h"

Transfer::Transfer(QObject* parent) : QObject(parent), MetaData() {
    setMetadata(Key::debitAccountId, QUuid{});
    setMetadata(Key::creditAccountId, QUuid{});
    setMetadata(Key::amount, 0.0);
    setMetadata(Key::date, QDate::currentDate());
    setMetadata(Key::description, QString{});
}

// --- Setters ---------------------------------------------------------------

void Transfer::setDebitAccountId(QUuid v) {
    if (debitAccountId() == v) return;
    setMetadata(Key::debitAccountId, v);
    emit debitAccountIdChanged();
    emit changed();
}

void Transfer::setCreditAccountId(QUuid v) {
    if (creditAccountId() == v) return;
    setMetadata(Key::creditAccountId, v);
    emit creditAccountIdChanged();
    emit changed();
}

void Transfer::setAmount(double v) {
    if (v < 0.0) v = 0.0;
    if (qFuzzyCompare(amount() + 1.0, v + 1.0)) return;
    setMetadata(Key::amount, v);
    emit amountChanged();
    emit changed();
}

void Transfer::setDate(QDate v) {
    if (date() == v) return;
    setMetadata(Key::date, v);
    emit dateChanged();
    emit changed();
}

void Transfer::setDescription(QString v) {
    if (description() == v) return;
    setMetadata(Key::description, v);
    emit descriptionChanged();
    emit changed();
}

// --- Validation / generation -------------------------------------------------

bool Transfer::isValid() const {
    return !debitAccountId().isNull() &&
           !creditAccountId().isNull() &&
           debitAccountId() != creditAccountId() &&
           amount() > 0.0 &&
           date().isValid();
}

QList<TransactionPtr> Transfer::generateTransactions() const {
    QList<TransactionPtr> transactions;

    if (!isValid())
        return transactions;

    TransactionPtr debitLeg = TransactionPtr::create();
    debitLeg->setValue(amount());
    debitLeg->setDate(date());
    debitLeg->setDescription(description());
    debitLeg->setSupport(OpenAccountEnums::Support::Transfer);
    debitLeg->setMovement(OpenAccountEnums::Movement::Debit);
    debitLeg->setAccountId(debitAccountId());
    debitLeg->setIsVisible(true);

    TransactionPtr creditLeg = TransactionPtr::create();
    creditLeg->setValue(amount());
    creditLeg->setDate(date());
    creditLeg->setDescription(description());
    creditLeg->setSupport(OpenAccountEnums::Support::Transfer);
    creditLeg->setMovement(OpenAccountEnums::Movement::Credit);
    creditLeg->setAccountId(creditAccountId());
    creditLeg->setIsVisible(true);

    transactions.append(debitLeg);
    transactions.append(creditLeg);

    return transactions;
}
