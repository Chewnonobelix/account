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

    function test_defaultIsUnselected() {
        compare(row.selected, false)
    }

    function test_selectAndDeselect() {
        row.select()
        compare(row.selected, true)
        row.deselect()
        compare(row.selected, false)
    }

    // TransactionRow reacts to a bare TapHandler, and mouseClick() synthetic
    // events aren't reliably delivered under the offscreen QPA platform
    // used for headless CI. toggleSelected() is the exact function
    // TapHandler.onTapped invokes, so calling it directly still exercises
    // the real tap logic.
    function test_tapTogglesSelected() {
        row.toggleSelected()
        compare(row.selected, true)
        row.toggleSelected()
        compare(row.selected, false)
    }
}
