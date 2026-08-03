import QtQuick

import "../Style" as Style
import "../Components" as Comp

// A single selectable row of a table/list: one line of text, highlighted
// when selected. The highlight color depends on rowState (Neutral uses the
// generic selection color, Positive/Negative use the success/danger washes).
Item {
    id: root

    enum RowState {
        Neutral,
        Positive,
        Negative
    }

    property string text: ""
    property bool selected: false
    property int rowState: TableRow.RowState.Neutral

    property Gradient selectedGradient: Style.OBStyle.selected
    property Gradient positiveGradient: Style.OBStyle.positive
    property Gradient negativeGradient: Style.OBStyle.negative

    readonly property Gradient highlightGradient: root.rowState === TableRow.RowState.Positive ? root.positiveGradient :
                                                   root.rowState === TableRow.RowState.Negative ? root.negativeGradient :
                                                   root.selectedGradient

    implicitWidth: Style.OBConstants.widthBig
    implicitHeight: Style.OBConstants.heightMedium

    Rectangle {
        anchors.fill: parent
        visible: root.selected
        gradient: root.highlightGradient
    }

    Comp.OBLabel {
        text: root.text
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: Style.OBConstants.leftMargins
        anchors.rightMargin: Style.OBConstants.rightMargins
        anchors.verticalCenter: parent.verticalCenter
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
