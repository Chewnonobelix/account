import QtQuick
import QtQuick.Controls

import "../Style" as Style

ComboBox {
    id: root

    implicitWidth: Style.OBConstants.widthMedium
    implicitHeight: Style.OBConstants.heightMedium

    property real borderRadius: Style.OBConstants.borderRadius
    property real borderWidth: Style.OBConstants.borderWidth
    property color borderColorNormal: Style.OBTheme.palette.outline
    property color borderColorFocused: Style.OBTheme.palette.outlineStrong
    property color backgroundColorNormal: Style.OBTheme.palette.surface
    property color backgroundColorDisabled: Style.OBTheme.palette.disabledLight

    background: Rectangle {
        radius: root.borderRadius
        color: root.enabled ? root.backgroundColorNormal : root.backgroundColorDisabled
        border.width: root.borderWidth
        border.color: (root.activeFocus || root.popup.visible) ? root.borderColorFocused : root.borderColorNormal
    }

    contentItem: Text {
        text: root.displayText
        font: root.font
        color: root.enabled ? Style.OBTheme.palette.textPrimary : Style.OBTheme.palette.textDisabled
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        leftPadding: Style.OBConstants.leftMargins
        rightPadding: root.indicator.width + Style.OBConstants.rightMargins
    }
}
