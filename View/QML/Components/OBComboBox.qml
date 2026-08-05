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
    property Gradient gradientNormal: Style.OBStyle.goldOut
    property Gradient gradientActive: Style.OBStyle.goldIn
    property Gradient gradientDisabled: Style.OBStyle.silver

    // Closed state looks like an OBButton; the popup contents are the
    // delegates themselves (also OBButton-styled), so the popup area stays
    // transparent instead of drawing a second boxed background behind them.
    background: Rectangle {
        radius: root.borderRadius
        border.width: root.borderWidth
        border.color: !root.enabled ? Style.OBTheme.palette.disabled :
                      root.popup.visible ? root.borderColorFocused : root.borderColorNormal
        gradient: !root.enabled ? root.gradientDisabled :
                  root.popup.visible ? root.gradientActive : root.gradientNormal
    }

    contentItem: OBLabel {
        text: root.displayText
        horizontalAlignment: Text.AlignHCenter
        textState: root.enabled ? OBLabel.TextState.Neutral : OBLabel.TextState.Disabled
        colorNeutral: Style.OBTheme.palette.accentContrast
        leftPadding: root.indicator.width + Style.OBConstants.leftMargins
        rightPadding: root.indicator.width + Style.OBConstants.rightMargins
    }

    // Marks the control as openable; the popup itself is closed/opened by
    // clicking anywhere on the control, this glyph doesn't handle input.
    indicator: OBLabel {
        text: ">"
        x: root.width - width - Style.OBConstants.rightMargins
        y: (root.height - height) / 2
        textState: root.enabled ? OBLabel.TextState.Neutral : OBLabel.TextState.Disabled
    }

    popup: Popup {
        y: root.height
        width: root.width
        implicitHeight: contentItem.implicitHeight
        padding: 0

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: root.popup.visible ? root.delegateModel : null
            currentIndex: root.highlightedIndex
            ScrollIndicator.vertical: ScrollIndicator {}
        }

        background: Rectangle {
            color: "transparent"
        }
    }

    delegate: ItemDelegate {
        id: delegateItem
        width: root.width
        text: modelData
        highlighted: root.highlightedIndex === index

        background: Rectangle {
            radius: root.borderRadius
            border.width: root.borderWidth
            border.color: !delegateItem.enabled ? Style.OBTheme.palette.disabled :
                          (delegateItem.highlighted || delegateItem.hovered) ? Style.OBTheme.palette.outlineStrong : Style.OBTheme.palette.outline
            gradient: !delegateItem.enabled ? Style.OBStyle.silver :
                      (delegateItem.highlighted || delegateItem.hovered) ? Style.OBStyle.goldIn : Style.OBStyle.goldOut
        }

        contentItem: OBLabel {
            text: delegateItem.text
            horizontalAlignment: Text.AlignHCenter
            textState: delegateItem.enabled ? OBLabel.TextState.Neutral : OBLabel.TextState.Disabled
            colorNeutral: Style.OBTheme.palette.accentContrast
            rightPadding: selectionIndicator.width + Style.OBConstants.rightMargins
        }

        // Non-interactive: selecting an entry is still done by clicking the
        // delegate itself, this is only a visual marker of root.currentValue.
        OBRadioButton {
            id: selectionIndicator
            x: delegateItem.width - width - Style.OBConstants.rightMargins
            y: (delegateItem.height - height) / 2
            enabled: false
            checked: modelData === root.currentValue
        }
    }
}
