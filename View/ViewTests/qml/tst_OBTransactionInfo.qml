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

    // OBTransactionInfo's movement/support fields are backed by these same
    // model types (MovementModel/SupportModel); it doesn't expose its own
    // internal instances, so the name<->value round trip is verified here
    // directly against the models it delegates to.
    MovementModel {
        id: movementModel
    }
    SupportModel {
        id: supportModel
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

    function test_movementModelNamesAndValuesLineUp() {
        compare(movementModel.textAt(movementModel.indexOfValue(OpenAccountEnums.Movement.Credit)), "Credit")
        compare(movementModel.textAt(movementModel.indexOfValue(OpenAccountEnums.Movement.Debit)), "Debit")
    }

    function test_supportModelNamesAndValuesLineUp() {
        compare(supportModel.textAt(supportModel.indexOfValue(OpenAccountEnums.Support.Transfer)), "Transfer")
        compare(supportModel.textAt(supportModel.indexOfValue(OpenAccountEnums.Support.Card)), "Card")
    }
}
