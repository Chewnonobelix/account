import QtQuick 2.14
import QtQuick.Controls 2.14
import "../Functionnal"

DoubleSpinBox {
    id: root
    
    ToolTip.delay: 500
    ToolTip.timeout: 1000
    ToolTip.visible: root.hovered

    font.family: AccountStyle.core.name
    font.pixelSize: AccountStyle.core.size
    clip: true


    up.indicator: Rectangle {
        x: root.mirrored ? 0 : parent.width - width
        height: parent.height
        width: parent.width * 0.25
        border.color: enabled ? "#21be2b" : "#bdbebf"
        gradient: enabled ? up.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton: AccountStyle.unselectView
        
        
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            cursorShape: parent.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
            
            onPressed: root.increase()
            
        }
        
        Text {
            
            text: ">"
            font.pixelSize: root.font.pixelSize * 2
            color: enabled ? "black" : "grey"
            anchors.fill: parent
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
    
    down.indicator: Rectangle {
        x: root.mirrored ? parent.width - width : 0
        height: parent.height
        width: parent.width * 0.25
        border.color: enabled ? "#21be2b" : "#bdbebf"
        gradient: enabled ? down.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton: AccountStyle.unselectView
        
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            cursorShape: parent.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
            
            onPressed: root.decrease()
        }
        
        Text {
            text: "<"
            font.pixelSize: root.font.pixelSize * 2
            color: enabled ? "black" : "grey"
            anchors.fill: parent
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
    
    background: Rectangle {
        color: "transparent"
        implicitWidth: 140
        border.color: "#bdbebf"
    }    
}
