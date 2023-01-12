import QtQuick 2.14
import QtQuick.Controls 2.14

ComboBox {
    id: root
    
    ToolTip.delay: 500
    ToolTip.timeout: 1000
    ToolTip.visible: root.hovered



    contentItem: Text {
        color: "black"
        font.family: AccountStyle.core.name
        font.pixelSize: AccountStyle.core.size
        text: root.currentText
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        Rectangle {
            gradient: AccountStyle.goldButton
            anchors.fill: parent
        }

    }
    clip: true

    delegate: ItemDelegate{
        id: del
        width: root.width
        text: textRole ? name : modelData
        
        contentItem: Text {
            color: "black"
            font.family: AccountStyle.core.name
            font.pixelSize: AccountStyle.core.size
            text: del.text
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
        }
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
