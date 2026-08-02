import QtQuick
import QtQuick.Controls

import "../Style" as Style

TextField {
    id: root

    implicitWidth: Style.OBConstants.widthMedium
    implicitHeight: Style.OBConstants.heightMedium

    leftPadding: Style.OBConstants.leftMargins
    rightPadding: Style.OBConstants.rightMargins
    topPadding: Style.OBConstants.topMargins / 2
    bottomPadding: Style.OBConstants.bottomMargins / 2

    property real borderRadius: Style.OBConstants.borderRadius
    property real borderWidth: Style.OBConstants.borderWidth
    property color borderColorNormal: Style.OBTheme.palette.outline
    property color borderColorFocused: Style.OBTheme.palette.outlineStrong
    property color backgroundColorNormal: Style.OBTheme.palette.surface
    property color backgroundColorDisabled: Style.OBTheme.palette.disabledLight

    color: root.enabled ? Style.OBTheme.palette.textPrimary : Style.OBTheme.palette.textDisabled
    placeholderTextColor: Style.OBTheme.palette.textDisabled
    selectionColor: Style.OBTheme.palette.selection
    selectedTextColor: Style.OBTheme.palette.textPrimary

    background: Rectangle {
        radius: root.borderRadius
        color: root.enabled ? root.backgroundColorNormal : root.backgroundColorDisabled
        border.width: root.borderWidth
        border.color: root.activeFocus ? root.borderColorFocused : root.borderColorNormal
    }
}
