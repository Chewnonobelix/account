import QtQuick
import QtQuick.Controls

import "../Style" as Style

Control {
    id: root

    implicitWidth: Style.OBConstants.widthLittle
    implicitHeight: Style.OBConstants.heightLittle

    // QtQuick.Controls' Switch is binary only; checkState carries the
    // extra Qt.PartiallyChecked state when tristate is enabled.
    property bool tristate: false
    property int checkState: Qt.Unchecked
    property bool checked: false

    property Gradient trackGradientOff: Style.OBStyle.silver
    property Gradient trackGradientOn: Style.OBStyle.goldIn
    property real trackOnOpacity: 0.75
    property Gradient thumbGradientEnabled: Style.OBStyle.goldOut
    property Gradient thumbGradientDisabled: Style.OBStyle.silver
    property color borderColor: Style.OBTheme.palette.outline
    property real borderWidth: Style.OBConstants.borderWidth

    onCheckedChanged: if (!tristate) checkState = checked ? Qt.Checked : Qt.Unchecked
    onCheckStateChanged: checked = checkState === Qt.Checked

    function nextCheckState() {
        if (tristate) {
            checkState = checkState === Qt.Unchecked ? Qt.PartiallyChecked :
                         checkState === Qt.PartiallyChecked ? Qt.Checked : Qt.Unchecked
        } else {
            checkState = checkState === Qt.Checked ? Qt.Unchecked : Qt.Checked
        }
    }

    background: Rectangle {
        id: track
        radius: height / 2
        gradient: root.trackGradientOff
        border.width: root.borderWidth
        border.color: root.borderColor

        Rectangle {
            id: fill
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            radius: track.radius
            gradient: root.trackGradientOn
            opacity: root.trackOnOpacity
            width: root.checkState === Qt.Unchecked ? 0 :
                   root.checkState === Qt.PartiallyChecked ? parent.width / 2 : parent.width

            Behavior on width {
                NumberAnimation { duration: 120; easing.type: Easing.OutQuad }
            }
        }

        Rectangle {
            id: thumb
            width: parent.height - 4
            height: parent.height - 4
            radius: width / 2
            anchors.verticalCenter: parent.verticalCenter
            gradient: root.enabled ? root.thumbGradientEnabled : root.thumbGradientDisabled
            x: root.checkState === Qt.Unchecked ? 2 :
               root.checkState === Qt.PartiallyChecked ? (parent.width - width) / 2 : parent.width - width - 2

            Behavior on x {
                NumberAnimation { duration: 120; easing.type: Easing.OutQuad }
            }
        }
    }

    TapHandler {
        enabled: root.enabled
        onTapped: root.nextCheckState()
    }
}
