import QtQuick
import QtQuick.Controls

import "../Style" as Style

Button {
    id: root

    implicitWidth: Style.OBConstants.widthMedium
    implicitHeight: Style.OBConstants.heightMedium

    property real borderRadius: Style.OBConstants.borderRadius
    property real borderWidth: Style.OBConstants.borderWidth
    property color borderColorNormal: Style.OBTheme.palette.outline
    property color borderColorPressed: Style.OBTheme.palette.outlineStrong
    property color borderColorDisabled: Style.OBTheme.palette.disabled
    property Gradient gradientNormal: Style.OBStyle.goldOut
    property Gradient gradientPressed: Style.OBStyle.goldIn
    property Gradient gradientDisabled: Style.OBStyle.silver

    background: Rectangle {
        radius: root.borderRadius
        border.width: root.borderWidth
        border.color: !root.enabled ? root.borderColorDisabled :
                      root.pressed ? root.borderColorPressed : root.borderColorNormal
        gradient: !root.enabled ? root.gradientDisabled :
                  root.pressed ? root.gradientPressed : root.gradientNormal
    }

    contentItem: OBLabel {
        text: root.text
        horizontalAlignment: Text.AlignHCenter
        textState: root.enabled ? OBLabel.TextState.Neutral : OBLabel.TextState.Disabled
    }
}
