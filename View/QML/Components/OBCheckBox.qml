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
    property color backgroundColor: Style.OBTheme.palette.surface
    property color checkedColor: Style.OBTheme.palette.accent
    property color partialColor: Style.OBTheme.palette.accentWarm

    indicator: Rectangle {
        implicitWidth: root.indicatorSize
        implicitHeight: root.indicatorSize
        x: root.leftPadding
        y: (root.height - height) / 2
        radius: root.borderRadius
        color: root.backgroundColor
        border.width: root.borderWidth
        border.color: root.borderColor

        Rectangle {
            anchors.centerIn: parent
            width: parent.width - 8
            height: parent.height - 8
            radius: root.borderRadius / 2
            visible: root.checkState !== Qt.Unchecked
            color: root.checkState === Qt.PartiallyChecked ? root.partialColor : root.checkedColor
        }
    }

    contentItem: Text {
        text: root.text
        font: root.font
        color: root.enabled ? Style.OBTheme.palette.textPrimary : Style.OBTheme.palette.textDisabled
        verticalAlignment: Text.AlignVCenter
        leftPadding: root.indicator.width + root.spacing
    }
}
