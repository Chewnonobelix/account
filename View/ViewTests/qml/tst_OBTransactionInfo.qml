import QtQuick
import QtTest

import OpenAccount.Model

import "../../QML/ComposedComponent" as Composed

TestCase {
    id: testCase
    name: "OBTransactionInfo"
    when: windowShown
    width: 400
    height: 400

    Transaction {
        id: creditTx
        name: "Salary"
        description: "Monthly salary"
        date: "2026-01-05"
        value: 2500
        movement: OpenAccountEnums.Movement.Credit
        support: OpenAccountEnums.Support.Transfer
    }

    Transaction {
        id: debitTx
        name: "Rent"
        description: "Monthly rent"
        date: "2026-01-06"
        value: 950
        movement: OpenAccountEnums.Movement.Debit
        support: OpenAccountEnums.Support.Card
    }

    Composed.OBTransactionInfo {
        id: info
        transaction: creditTx
    }

    function test_noTransactionIsNotCredit() {
        info.transaction = null
        compare(info.isCredit, false)
        info.transaction = creditTx
    }

    function test_creditTransactionIsCredit() {
        info.transaction = creditTx
        compare(info.isCredit, true)
    }

    function test_debitTransactionIsNotCredit() {
        info.transaction = debitTx
        compare(info.isCredit, false)
        info.transaction = creditTx
    }

    function test_defaultIsReadOnly() {
        compare(info.readOnly, true)
    }

    function test_readOnlyCanBeToggled() {
        info.readOnly = false
        compare(info.readOnly, false)
        info.readOnly = true
    }

    function test_movementNamesAndValuesLineUp() {
        compare(info.movementNames.length, info.movementValues.length)
        compare(info.movementValues[info.movementNames.indexOf("Credit")], OpenAccountEnums.Movement.Credit)
        compare(info.movementValues[info.movementNames.indexOf("Debit")], OpenAccountEnums.Movement.Debit)
    }

    function test_supportNamesAndValuesLineUp() {
        compare(info.supportNames.length, info.supportValues.length)
        compare(info.supportValues[info.supportNames.indexOf("Transfer")], OpenAccountEnums.Support.Transfer)
        compare(info.supportValues[info.supportNames.indexOf("Card")], OpenAccountEnums.Support.Card)
    }
}
