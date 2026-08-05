import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

import "../Style" as Style

RadioButton {
    id: root

    implicitHeight: Style.OBConstants.heightLittle

    property real indicatorSize: Style.OBConstants.heightLittle
    property real borderWidth: Style.OBConstants.borderWidth
    property color borderColor: Style.OBTheme.palette.outline
    property Gradient backgroundGradient: Style.OBStyle.silver
    property RadialGradient checkedGradient: Style.OBStyle.radioSelected

    indicator: Rectangle {
        implicitWidth: root.indicatorSize
        implicitHeight: root.indicatorSize
        x: root.leftPadding
        y: (root.height - height) / 2
        radius: width / 2
        gradient: root.backgroundGradient
        border.width: root.borderWidth
        border.color: root.borderColor

        Shape {
            id: selectedDot
            anchors.centerIn: parent
            width: parent.width - 8
            height: parent.height - 8
            visible: root.checked

            ShapePath {
                fillGradient: root.checkedGradient
                strokeColor: "transparent"

                PathAngleArc {
                    centerX: selectedDot.width / 2
                    centerY: selectedDot.height / 2
                    radiusX: selectedDot.width / 2
                    radiusY: selectedDot.height / 2
                    startAngle: 0
                    sweepAngle: 360
                }
            }
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
