import QtQuick
import QtQuick.Controls

import "../Style" as Style

SpinBox {
    id: root

    implicitWidth: Style.OBConstants.widthMedium
    implicitHeight: Style.OBConstants.heightMedium

    editable: true

    // SpinBox only natively handles integers: from/to/stepSize/value are
    // scaled by decimalFactor so realValue/realFrom/realTo/realStepSize can
    // carry fractional numbers.
    property int decimals: 2
    readonly property real decimalFactor: Math.pow(10, decimals)

    property real realFrom: 0
    property real realTo: 100
    property real realStepSize: 1
    property real realValue: 0

    from: realFrom * decimalFactor
    to: realTo * decimalFactor
    stepSize: realStepSize * decimalFactor

    onRealValueChanged: value = Math.round(realValue * decimalFactor)
    onValueChanged: realValue = value / decimalFactor

    validator: DoubleValidator {
        bottom: Math.min(root.realFrom, root.realTo)
        top: Math.max(root.realFrom, root.realTo)
        decimals: root.decimals
        notation: DoubleValidator.StandardNotation
    }

    textFromValue: function (value, locale) {
        return Number(value / root.decimalFactor).toLocaleString(locale, "f", root.decimals)
    }

    valueFromText: function (text, locale) {
        return Math.round(Number.fromLocaleString(locale, text) * root.decimalFactor)
    }

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
        border.color: root.activeFocus ? root.borderColorFocused : root.borderColorNormal
    }

    property real indicatorWidth: Style.OBConstants.heightMedium
    property color indicatorColorNormal: Style.OBTheme.palette.surfaceRaised
    property color indicatorColorPressed: Style.OBTheme.palette.accent
    property color indicatorTextColor: Style.OBTheme.palette.textPrimary

    up.indicator: Rectangle {
        x: root.mirrored ? 0 : root.width - width
        implicitWidth: root.indicatorWidth
        height: root.height
        radius: root.borderRadius
        color: root.up.pressed ? root.indicatorColorPressed : root.indicatorColorNormal
        border.width: root.borderWidth
        border.color: root.borderColorNormal
        opacity: root.value < root.to ? 1 : 0.5

        Text {
            text: "+"
            anchors.centerIn: parent
            font.pixelSize: root.font.pixelSize * 1.2
            color: root.indicatorTextColor
        }
    }

    down.indicator: Rectangle {
        x: root.mirrored ? root.width - width : 0
        implicitWidth: root.indicatorWidth
        height: root.height
        radius: root.borderRadius
        color: root.down.pressed ? root.indicatorColorPressed : root.indicatorColorNormal
        border.width: root.borderWidth
        border.color: root.borderColorNormal
        opacity: root.value > root.from ? 1 : 0.5

        Text {
            text: "-"
            anchors.centerIn: parent
            font.pixelSize: root.font.pixelSize * 1.2
            color: root.indicatorTextColor
        }
    }
}
