import QtQuick 2.13
import QtQuick.Controls 2.5
import QtQuick.Window 2.12
import QtQuick.Layouts 1.13
import "../Style" as S
import "../Core" as C

Window {
    property int freqId: -1
    property int freqGroup: -1
    
    signal s_generate(date fBegin, date fEnd)
    
    flags: Qt.Popup | Qt.NoDropShadowWindowHint
        
    
    maximumHeight: Screen.height - 1
    maximumWidth: Screen.width - 1
    minimumHeight: Screen.height - 1
    minimumWidth: Screen.width - 1
    x: 0
    y: 0
    color: "transparent"
    
    Component.onCompleted: console.log("generate", maximumHeight, maximumWidth)
//    onVisibleChanged: visibility = Window.Maximized
    S.AccountStyle{
        id: pageStyle
    }
    
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.NoButton
        propagateComposedEvents: true
        preventStealing: false
    }

    Rectangle {
        gradient: pageStyle.backgroundGradient
//        anchors.fill: parent
        anchors.centerIn: parent
        width: 300
        height: 200
        
        GridLayout {
            
            
            Label {
                Layout.row: 0
                Layout.column: 0
                text: qsTr("From")
            }
            
            C.CalendarButton {
                Layout.row: 1
                Layout.column: 0
                id: from
                
            }
            
            Label {
                Layout.row: 0
                Layout.column: 1
                text:qsTr("To")
            }
            
            C.CalendarButton {
                Layout.row: 1
                Layout.column: 1
                id: to
            }
            
            Button {
                Layout.row: 2
                Layout.column: 0
                text: qsTr("Generate")
            }
            
            Button {
                Layout.row: 2
                Layout.column: 1
                text: qsTr("Cancel")
            }
        }
    }
}
