import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Controls 1.4 as C1
import QtQuick.Controls 2.5
import "../Style"
import "../Core"

Window {
    flags: Qt.Popup | Qt.NoDropShadowWindowHint
    
    Component.onCompleted: console.log(screen, flags, Qt.Popup, Qt.FramelessWindowHint)
    
    x: screen.width / 2 - width / 2
    y: screen.height / 2 - height / 2
    
    minimumHeight: 350
    minimumWidth: 350
    
    AccountStyle {
        id: pageStyle
    }

    //    width: (col1.width + col2.width) * 1.10
    //    height: (col2.height)* 1.10
    id: main
    color: "transparent"
    
    Rectangle {
        width: (( col2.x + col2.width) - col1.x) * 1.05
        height: (( row1.y + row1.height) - col2.y) * 1.05   
        
        border.color: "gold"
        gradient: pageStyle.backgroundGradient
        Column {
            id: col1
            anchors.topMargin: 10
            anchors.leftMargin: 10
            
            Label {
                id: l_reference
                text: "Reference Date"
            }
            
            CalendarButton {
            }
        }
        
        Column {
            id: col2
            anchors.topMargin: 10        
            anchors.left: col1.right
            anchors.verticalCenter: col1.verticalCenter
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            Label {
                id: l_frequency
                text: "Frequency"
            }
            
            ComboBox {
                id: frequency
                model: ["unique" ,"day", "week", "month", "quarter", "year"]
                
            }
        }
        
        Row {
            id: row1
            anchors.top: col2.bottom
            anchors.topMargin: 10
            anchors.rightMargin: 10
            anchors.right: col2.right
            spacing: 10
            Button {
                text: "ok"
            }
            
            Button {            
                text: "Cancel"
                onClicked: main.close()
            }
        }
    }
}
