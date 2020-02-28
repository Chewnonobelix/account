import QtQuick 2.14
import QtQuick.Controls 2.14

Button {
    id: root
        
    font.family: AccountStyle.core.name
    font.pixelSize: AccountStyle.core.size
    
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