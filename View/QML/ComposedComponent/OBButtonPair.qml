import QtQuick
import QtQuick.Layouts

import "../Style" as Style
import "../Components" as Comp

// Two-button row for paired actions — decrement/increment, cancel/accept,
// previous/next, etc. Text and enabled state are configurable per side;
// behavior is left entirely to the caller via the leftClicked/rightClicked
// signals.
RowLayout {
    id: root

    spacing: Style.OBConstants.horizontalSpacing / 2

    property string leftText: "-"
    property string rightText: "+"
    property bool leftEnabled: true
    property bool rightEnabled: true
    // Opt-in: both buttons split the row evenly instead of sizing to their
    // own text (e.g. a popup footer meant to span edge-to-edge, like
    // OBValidateTransactionsPopup's Later/Validate row). Off by default so
    // existing right-aligned/content-sized callers are unaffected.
    property bool stretch: false

    readonly property alias leftButton: leftButtonItem
    readonly property alias rightButton: rightButtonItem

    signal leftClicked()
    signal rightClicked()

    Comp.OBButton {
        id: leftButtonItem
        Layout.fillWidth: root.stretch
        text: root.leftText
        enabled: root.leftEnabled
        onClicked: root.leftClicked()
    }

    Comp.OBButton {
        id: rightButtonItem
        Layout.fillWidth: root.stretch
        text: root.rightText
        enabled: root.rightEnabled
        onClicked: root.rightClicked()
    }
}
