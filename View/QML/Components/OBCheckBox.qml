import QtQuick
import QtQuick.Controls

import "../Style" as Style

CheckBox {
    id: root

    implicitHeight: Style.OBConstants.heightLittle

    property real indicatorSize: Style.OBConstants.heightLittle
    property real borderRadius: Style.OBConstants.borderRadius
    property real borderWidth: Style.OBConstants.borderWidth
    property color borderColor: Style.OBTheme.palette.outline
    property Gradient backgroundGradient: Style.OBStyle.silver
    property Gradient checkedGradient: Style.OBStyle.goldOut
    property Gradient partialGradient: Style.OBStyle.goldIn

    indicator: Rectangle {
        implicitWidth: root.indicatorSize
        implicitHeight: root.indicatorSize
        x: root.leftPadding
        y: (root.height - height) / 2
        radius: root.borderRadius
        gradient: root.backgroundGradient
        border.width: root.borderWidth
        border.color: root.borderColor

        Rectangle {
            anchors.centerIn: parent
            width: parent.width - 8
            height: parent.height - 8
            radius: root.borderRadius / 2
            visible: root.checkState !== Qt.Unchecked
            gradient: root.checkState === Qt.PartiallyChecked ? root.partialGradient : root.checkedGradient
        }
    }

    contentItem: Text {
        text: root.text
        font: root.font
        color: root.enabled ? Style.OBTheme.palette.textPrimary : Style.OBTheme.palette.textDisabled
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        leftPadding: root.indicator.width + root.spacing
    }
}
