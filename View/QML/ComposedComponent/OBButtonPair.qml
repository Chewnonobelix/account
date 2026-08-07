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

    readonly property alias leftButton: leftButtonItem
    readonly property alias rightButton: rightButtonItem

    signal leftClicked()
    signal rightClicked()

    Comp.OBButton {
        id: leftButtonItem
        text: root.leftText
        enabled: root.leftEnabled
        onClicked: root.leftClicked()
    }

    Comp.OBButton {
        id: rightButtonItem
        text: root.rightText
        enabled: root.rightEnabled
        onClicked: root.rightClicked()
    }
}
