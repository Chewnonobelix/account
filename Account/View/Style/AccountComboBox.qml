import QtQuick 2.14
import QtQuick.Controls 2.14

ComboBox {
    id: root
    
    Rectangle {
        gradient: AccountStyle.goldButton
        anchors.fill: parent
    }
    
    font.family: AccountStyle.core.name
    font.pixelSize: AccountStyle.core.size
    
    delegate: ItemDelegate{
        id: del
        width: root.width
        text: name
        
        font.family: AccountStyle.core.name
        font.pixelSize: AccountStyle.core.size
        
        background: Rectangle {
            gradient: AccountStyle.goldButton
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                acceptedButtons: Qt.NoButton
            }
        }
    }
}
