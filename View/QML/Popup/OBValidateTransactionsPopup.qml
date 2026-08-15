import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import OpenAccount.Model

import "../Style" as Style
import "../Components" as Comp
import "../ComposedComponent" as Composed

// Popup shown once estimated (not-yet-confirmed) transactions fall due:
// every Transaction with estimated == true whose date has already passed,
// each with a checkbox — checked by default — and an editable amount, to
// include it in this validation pass.
//
// "Later" closes without changing anything. "Validate" applies the
// decision implied by each row's checkbox: checked means the estimate
// happened (at whatever amount the row was edited to, defaulting to the
// original estimate), so it writes that value and flips estimated to false
// in place (mirroring OBBudgetPage's/OBTransactionInfo's editable fields);
// unchecked means it didn't, so the transaction is removed from
// transactionModel entirely instead of being left pending forever — its
// edited amount, if any, is simply discarded along with it.
Popup {
    id: root

    property TransactionListModel transactionModel: null

    implicitWidth: Style.OBConstants.widthBig * 2 + Style.OBConstants.leftMargins * 2
    implicitHeight: Style.OBConstants.heightBig * 2 + Style.OBConstants.heightMedium * 6

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    padding: 0

    signal validated(var transactions)
    signal discarded(var transactions)
    signal rejected()

    // Every not-yet-confirmed Transaction (estimated == true) whose date
    // has already passed — the only ones this popup is for. Filtering
    // (including reacting to any Transaction's estimated/date changing) is
    // done in C++ by EstimatedTransactionFilterProxyModel, incrementally
    // per row instead of this popup rescanning the whole transactionModel
    // on every change.
    EstimatedTransactionFilterProxyModel {
        id: pendingProxy
        sourceModel: root.transactionModel
    }

    // rowRepeater is a plain Repeater (not a virtualized ListView), so
    // itemAt() reliably reaches every row regardless of scroll position —
    // the expected row count here (transactions awaiting confirmation) is
    // small enough that this doesn't need ListView's virtualization.
    function applyValidation() {
        // Collected in a first pass, mutated in a second: flipping
        // transaction.estimated (or removing the transaction outright)
        // immediately would shrink pendingProxy (it filters on exactly
        // that flag/on transactionModel's contents) mid-loop, which
        // re-indexes rowRepeater's remaining delegates out from under "i"
        // and silently skips rows.
        const toConfirm = []
        const toDiscard = []
        for (let i = 0; i < rowRepeater.count; ++i) {
            const row = rowRepeater.itemAt(i)
            if (!row) continue
            if (row.included)
                toConfirm.push({ transaction: row.transaction, value: row.editedValue })
            else
                toDiscard.push(row.transaction)
        }

        for (let i = 0; i < toConfirm.length; ++i) {
            toConfirm[i].transaction.value = toConfirm[i].value
            toConfirm[i].transaction.estimated = false
        }

        if (root.transactionModel) {
            for (let i = 0; i < toDiscard.length; ++i)
                root.transactionModel.removeTransaction(toDiscard[i].id)
        }

        root.validated(toConfirm.map(function (entry) { return entry.transaction }))
        root.discarded(toDiscard)
        root.close()
    }

    background: Rectangle {
        radius: Style.OBConstants.borderRadius
        border.width: Style.OBConstants.borderWidth
        border.color: Style.OBTheme.palette.outline
        gradient: Style.OBStyle.background
    }

    contentItem: ColumnLayout {
        spacing: Style.OBConstants.verticalSpacing / 2

        Comp.OBHeader {
            text: qsTr("VALIDATION")
            Layout.fillWidth: true
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            ScrollBar.vertical: Comp.OBScrollBar {}

            Column {
                width: root.availableWidth
                spacing: Style.OBConstants.verticalSpacing / 2

                Repeater {
                    id: rowRepeater
                    // An int model: each delegate looks its own transaction
                    // up via pendingProxy.at(index) rather than getting it
                    // handed through modelData, since the model here is a
                    // row count, not the transactions themselves.
                    model: pendingProxy.count

                    delegate: Rectangle {
                        id: rowRoot
                        // Local UI state: whether this row is part of the
                        // batch "Validate" will confirm, and what amount it
                        // confirms at — independent of transaction.value/
                        // estimated themselves, which only change once the
                        // user actually clicks Validate.
                        property bool included: true
                        readonly property var transaction: pendingProxy.at(index)
                        readonly property alias editedValue: amountSpin.realValue

                        width: parent ? parent.width : 0
                        height: Style.OBConstants.heightMedium + Style.OBConstants.borderWidth * 2
                        radius: Style.OBConstants.borderRadius
                        border.width: Style.OBConstants.borderWidth
                        border.color: Style.OBTheme.palette.outline
                        color: "transparent"

                        Row {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.leftMargin: Style.OBConstants.leftMargins / 2
                            anchors.rightMargin: Style.OBConstants.rightMargins / 2
                            spacing: Style.OBConstants.horizontalSpacing / 2

                            Comp.OBCheckBox {
                                anchors.verticalCenter: parent.verticalCenter
                                width: parent.width - amountSpin.width - parent.spacing
                                checked: rowRoot.included
                                onCheckedChanged: rowRoot.included = checked
                                text: rowRoot.transaction ?
                                      Qt.formatDate(rowRoot.transaction.date, "yyyy-MM-dd") + " - " + rowRoot.transaction.description : ""
                            }

                            // Estimated amounts are predictions; lets the
                            // user correct it to what actually happened
                            // before it's written back on Validate. Only
                            // relevant while the row is actually going to
                            // be confirmed — unchecked rows are discarded
                            // on Validate, so editing their amount would be
                            // a no-op.
                            Comp.OBSpinBox {
                                id: amountSpin
                                anchors.verticalCenter: parent.verticalCenter
                                enabled: rowRoot.included
                                decimals: 2
                                realFrom: 0
                                realTo: 1000000
                                realValue: rowRoot.transaction ? rowRoot.transaction.value : 0
                            }
                        }
                    }
                }

                Comp.OBLabel {
                    width: parent.width
                    height: Style.OBConstants.heightMedium
                    visible: pendingProxy.count === 0
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("No estimated transaction to validate")
                    textState: Comp.OBLabel.TextState.Disabled
                }
            }
        }

        Composed.OBButtonPair {
            Layout.fillWidth: true
            stretch: true
            leftText: qsTr("Later")
            rightText: qsTr("Validate")
            onLeftClicked: {
                root.rejected()
                root.close()
            }
            onRightClicked: root.applyValidation()
        }
    }
}
