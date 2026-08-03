import QtQuick
import QtTest

import OpenAccount.Model

import "../../QML/ComposedComponent" as Composed

TestCase {
    id: testCase
    name: "TransactionRow"
    when: windowShown
    width: 400
    height: 100

    Transaction {
        id: creditTx
        name: "Salary"
        description: "Monthly salary"
        date: "2026-01-05"
        value: 2500
        movement: OpenAccountEnums.Movement.Credit
    }

    Transaction {
        id: debitTx
        name: "Rent"
        description: "Monthly rent"
        date: "2026-01-06"
        value: 950
        movement: OpenAccountEnums.Movement.Debit
    }

    Composed.TransactionRow {
        id: row
        transaction: creditTx
    }

    function test_noTransactionIsNotCredit() {
        row.transaction = null
        compare(row.isCredit, false)
    }

    function test_creditTransactionIsCredit() {
        row.transaction = creditTx
        compare(row.isCredit, true)
        compare(row.highlightGradient, row.positiveGradient)
    }

    function test_debitTransactionIsNotCredit() {
        row.transaction = debitTx
        compare(row.isCredit, false)
        compare(row.highlightGradient, row.negativeGradient)
    }
}
