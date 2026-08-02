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

    leftPadding: down.indicator ? down.indicator.width : 0
    rightPadding: up.indicator ? up.indicator.width : 0

    contentItem: TextInput {
        text: root.textFromValue(root.value, root.locale)
        font: root.font
        color: root.enabled ? Style.OBTheme.palette.textPrimary : Style.OBTheme.palette.textDisabled
        selectionColor: Style.OBTheme.palette.selection
        selectedTextColor: Style.OBTheme.palette.textPrimary
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        readOnly: !root.editable
        validator: root.validator
        inputMethodHints: Qt.ImhFormattedNumbersOnly
        clip: width < implicitWidth
    }

    down.indicator: OBButton {
        x: root.mirrored ? root.width - width : 0
        y: 0
        implicitWidth: root.indicatorWidth
        implicitHeight: root.height
        text: "-"
        enabled: root.enabled && root.value > root.from
        onClicked: root.decrease()
    }

    up.indicator: OBButton {
        x: root.mirrored ? 0 : root.width - width
        y: 0
        implicitWidth: root.indicatorWidth
        implicitHeight: root.height
        text: "+"
        enabled: root.enabled && root.value < root.to
        onClicked: root.increase()
    }
}
