import QtQuick
import QtQuick.Controls

import "../Style" as Style

ScrollBar {
    id: root

    property real thickness: Style.OBConstants.scrollBarThickness
    property real handleRadius: root.thickness / 2
    property real borderWidth: Style.OBConstants.borderWidth
    property color borderColor: Style.OBTheme.palette.outline
    property Gradient grooveGradient: Style.OBStyle.silver
    property Gradient handleGradientNormal: Style.OBStyle.goldOut
    property Gradient handleGradientActive: Style.OBStyle.goldIn
    property Gradient handleGradientDisabled: Style.OBStyle.silver

    // Mirrors the Basic style's own ScrollBar: only draw when the policy
    // forces it, or while the handle is being interacted with and there is
    // actually more content than fits (size < 1.0).
    readonly property bool shown: root.policy === ScrollBar.AlwaysOn || (root.active && root.size < 1.0)

    implicitWidth: root.orientation === Qt.Vertical ? root.thickness : Style.OBConstants.widthBig
    implicitHeight: root.orientation === Qt.Vertical ? Style.OBConstants.widthBig : root.thickness

    background: Rectangle {
        radius: root.handleRadius
        gradient: root.grooveGradient
        opacity: root.shown ? 0.4 : 0.0

        Behavior on opacity {
            NumberAnimation { duration: 120; easing.type: Easing.OutQuad }
        }
    }

    contentItem: Rectangle {
        implicitWidth: root.thickness
        implicitHeight: root.thickness
        radius: root.handleRadius
        border.width: root.borderWidth
        border.color: root.borderColor
        gradient: !root.enabled ? root.handleGradientDisabled :
                  (root.pressed || root.active) ? root.handleGradientActive : root.handleGradientNormal
        opacity: root.shown ? 1.0 : 0.0

        Behavior on opacity {
            NumberAnimation { duration: 120; easing.type: Easing.OutQuad }
        }
    }
}
