import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4 as C1
import QtQuick.Controls 2.5
import "../Style"
import "../Core"

Window {
    flags: Qt.Popup | Qt.NoDropShadowWindowHint
        
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
    
    property string budgetName

    Rectangle {
        width: (( col2.x + col2.width) - col1.x) * 1.05
        height: (( row1.y + row1.height) - col2.y) * 1.05   
        
        anchors.leftMargin: 10
        anchors.topMargin: 10
        border.color: "gold"
        gradient: pageStyle.backgroundGradient
        Column {
            id: col1
            topPadding: 10
            rightPadding: 10
            leftPadding: 10

            spacing: 10             
            Label {
                Layout.alignment: Qt.AlignCenter
                anchors.topMargin: 10
                anchors.leftMargin: 10
                
                id: l_reference
                text: qsTr("Reference Date")
                fontSizeMode: Text.Fit
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
            }
            
            CalendarButton {
                id: cButton
                objectName: "cButton"
                Layout.alignment: Qt.AlignCenter
            }
        }
        
        Column {
            id: col2
//            anchors.topMargin: 10        
            anchors.left: col1.right
            anchors.verticalCenter: col1.verticalCenter
            
            topPadding: 10
            rightPadding: 10
            leftPadding: 10
            spacing: 10             
            
            Label {
                anchors.topMargin: 10
                anchors.leftMargin: 10
                
                id: l_frequency
                text: qsTr("Target")
                fontSizeMode: Text.Fit
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                
                Layout.alignment: Qt.AlignCenter
            }
            
            DoubleSpinBox {
                id: targetValue
                objectName: "targetValue"                
                Layout.alignment: Qt.AlignCenter
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
                objectName: "okButton"
                
                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }
            }
            
            Button {            
                text: "Cancel"
                onClicked: main.close()

                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }
                
            }
        }
    }
}
