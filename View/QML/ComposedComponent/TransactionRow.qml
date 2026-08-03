import QtQuick

import OpenAccount.Model

import "../Style" as Style
import "../Components" as Comp

// A single transaction row for a table/list: direction, name, date,
// description and amount. The row is tinted based on the bound Transaction's
// movement (Credit -> positive/success wash, Debit -> negative/danger wash).
Item {
    id: root

    property Transaction transaction: null

    readonly property bool isCredit: root.transaction !== null &&
                                      root.transaction.movement === OpenAccountEnums.Movement.Credit

    property Gradient positiveGradient: Style.OBStyle.positive
    property Gradient negativeGradient: Style.OBStyle.negative

    readonly property Gradient highlightGradient: root.isCredit ? root.positiveGradient : root.negativeGradient
    readonly property color directionColor: root.isCredit ? Style.OBTheme.palette.success : Style.OBTheme.palette.danger

    implicitWidth: contentRow.implicitWidth + 2 * Style.OBConstants.leftMargins
    implicitHeight: Style.OBConstants.heightMedium

    Rectangle {
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
            color: root.directionColor
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
}
