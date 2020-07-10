import QtQuick 2.14
import QtQuick.Controls 2.14

CheckBox {
    id: root

    font.family: AccountStyle.core.name    
    font.pixelSize: AccountStyle.core.size
    clip: true
    background: MouseArea {
        z: -1
        acceptedButtons: Qt.NoButton
        cursorShape: Qt.PointingHandCursor
    }
}
