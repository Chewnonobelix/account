import QtQuick 2.14
import QtQuick.Controls 2.14

Button {
    id: root

    clip: true
    font.family: AccountStyle.core.name
    font.pixelSize: AccountStyle.core.size

    ToolTip.delay: 500
    ToolTip.timeout: 1000
    ToolTip.visible: root.hovered

    contentItem: Text {
        color: "black"
        font.family: AccountStyle.core.name
        font.pixelSize: AccountStyle.core.size
        text: root.text
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
    }

    background: Rectangle {
        gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
        border.color: "darkgoldenrod"
        
        MouseArea {
            z: -1
            anchors.fill: parent
            cursorShape: parent.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
            acceptedButtons: Qt.NoButton
        }        
    }
}
