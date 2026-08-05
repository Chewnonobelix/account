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

    // icon.source/width/height come from AbstractButton's built-in grouped
    // "icon" property; we render it ourselves since contentItem is custom.
    property real iconSpacing: Style.OBConstants.horizontalSpacing / 2
    readonly property alias iconItem: iconImage

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
        colorNeutral: Style.OBTheme.palette.accentContrast
        rightPadding: root.icon.source.toString().length > 0 ? iconImage.width + root.iconSpacing : 0
    }

    Image {
        id: iconImage
        source: root.icon.source
        visible: source.toString().length > 0
        width: root.icon.width > 0 ? root.icon.width : Style.OBConstants.heightLittle
        height: root.icon.height > 0 ? root.icon.height : Style.OBConstants.heightLittle
        fillMode: Image.PreserveAspectFit
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: Style.OBConstants.rightMargins / 2
    }
}
