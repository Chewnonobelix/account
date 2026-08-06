import QtQuick

import OpenAccount.Model

import "../Style" as Style
import "../Components" as Comp

// A single selectable transaction row for a table/list: direction, name,
// date, description and amount. When selected, the row is tinted based on
// the bound Transaction's movement (Credit -> positive/success wash,
// Debit -> negative/danger wash).
Item {
    id: root

    property Transaction transaction: null
    property bool selected: false

    readonly property bool isCredit: root.transaction !== null &&
                                      root.transaction.movement === OpenAccountEnums.Movement.Credit

    property Gradient positiveGradient: Style.OBStyle.positive
    property Gradient negativeGradient: Style.OBStyle.negative

    property Gradient highlightGradient: root.negativeGradient
    property color directionColor: Style.OBTheme.palette.danger

    implicitWidth: contentRow.implicitWidth + 2 * Style.OBConstants.leftMargins
    implicitHeight: Style.OBConstants.heightMedium

    state: root.isCredit ? "credit" : "debit"

    states: [
        State {
            name: "credit"
            PropertyChanges { target: root; highlightGradient: root.positiveGradient; directionColor: Style.OBTheme.palette.success }
        },
        State {
            name: "debit"
            PropertyChanges { target: root; highlightGradient: root.negativeGradient; directionColor: Style.OBTheme.palette.danger }
        }
    ]

    // Direction tint, shown only while the row is selected.
    Rectangle {
        visible: root.selected
        anchors.fill: parent
        gradient: root.highlightGradient
    }

    Row {
        id: contentRow
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: Style.OBConstants.leftMargins
        anchors.rightMargin: Style.OBConstants.rightMargins
        spacing: Style.OBConstants.horizontalSpacing

        Comp.OBLabel {
            width: Style.OBConstants.widthLittle / 2
            text: root.isCredit ? "+" : "-"
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        Comp.OBLabel {
            width: Style.OBConstants.widthMedium
            text: root.transaction ? root.transaction.name : ""
            anchors.verticalCenter: parent.verticalCenter
        }

        Comp.OBLabel {
            width: Style.OBConstants.widthLittle
            text: root.transaction ? Qt.formatDate(root.transaction.date, "yyyy-MM-dd") : ""
            anchors.verticalCenter: parent.verticalCenter
        }

        Comp.OBLabel {
            width: Style.OBConstants.widthBig
            text: root.transaction ? root.transaction.description : ""
            anchors.verticalCenter: parent.verticalCenter
        }

        Comp.OBLabel {
            width: Style.OBConstants.widthLittle
            text: root.transaction ? (root.isCredit ? "+" : "-") + Math.abs(root.transaction.value).toFixed(2) : ""
            font.bold: true
            color: root.directionColor
            horizontalAlignment: Text.AlignRight
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    function select() {
        root.selected = true
    }

    function deselect() {
        root.selected = false
    }

    function toggleSelected() {
        root.selected = !root.selected
    }

    TapHandler {
        gesturePolicy: TapHandler.ReleaseWithinBounds
        onTapped: root.toggleSelected()
    }
}
