import QtQuick 2.14
import QtQuick.Controls 2.14

CheckBox {
    id: root

    ToolTip.delay: 500
    ToolTip.timeout: 1000
    ToolTip.visible: root.hovered


    font.family: AccountStyle.core.name    
    font.pixelSize: AccountStyle.core.size
    clip: true
    background: MouseArea {
        z: -1
        acceptedButtons: Qt.NoButton
        cursorShape: Qt.PointingHandCursor
    }
}
