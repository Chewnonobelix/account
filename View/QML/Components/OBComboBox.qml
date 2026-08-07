import QtQuick
import QtQuick.Controls

import "../Style" as Style

ComboBox {
    id: root

    enum PopupOrientation {
        Vertical,
        Horizontal
    }

    implicitWidth: Style.OBConstants.widthMedium
    implicitHeight: Style.OBConstants.heightMedium

    property int popupOrientation: OBComboBox.PopupOrientation.Vertical

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
        id: comboPopup
        padding: 0

        // Popup isn't an Item, so it can't own state/states itself; the
        // ListView below (its contentItem) drives both its own orientation
        // and the popup's geometry via PropertyChanges.
        contentItem: ListView {
            id: popupListView
            clip: true
            implicitWidth: contentWidth
            implicitHeight: contentHeight
            model: root.popup.visible ? root.delegateModel : null
            currentIndex: root.highlightedIndex
            ScrollIndicator.vertical: ScrollIndicator {}
            ScrollIndicator.horizontal: ScrollIndicator {}

            state: root.popupOrientation === OBComboBox.PopupOrientation.Horizontal ? "horizontal" : "vertical"

            states: [
                State {
                    name: "vertical"
                    PropertyChanges { target: popupListView; orientation: ListView.Vertical }
                    PropertyChanges { target: comboPopup; x: 0; y: root.height; width: root.width; implicitHeight: popupListView.contentHeight }
                },
                State {
                    name: "horizontal"
                    PropertyChanges { target: popupListView; orientation: ListView.Horizontal }
                    PropertyChanges { target: comboPopup; x: root.width; y: 0; width: popupListView.contentWidth; implicitHeight: root.height }
                }
            ]
        }

        background: Rectangle {
            color: "transparent"
        }
    }

    delegate: ItemDelegate {
        id: delegateItem
        // modelData is only synthesized for single-role models (e.g. a plain
        // string list, or EnumListModel); a multi-role model must say which
        // role holds the label via textRole, same as stock ComboBox.
        text: root.textRole ? model[root.textRole] : modelData
        highlighted: root.highlightedIndex === index

        state: root.popupOrientation === OBComboBox.PopupOrientation.Horizontal ? "horizontal" : "vertical"

        states: [
            State {
                name: "vertical"
                PropertyChanges { target: delegateItem; width: root.width }
            },
            State {
                name: "horizontal"
                PropertyChanges { target: delegateItem; height: root.height }
            }
        ]

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
            checked: (root.valueRole ? model[root.valueRole] : (root.textRole ? model[root.textRole] : modelData)) === root.currentValue
        }
    }
}
