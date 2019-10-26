import QtQuick 2.13
import QtQuick.Controls 2.5
import QtQuick.Window 2.12
import QtQuick.Layouts 1.13
import "../Style" as S
import "../Core" as C

Window {
    property int freqId: -1
    property int freqGroup: -1
    
    signal s_generate(string fBegin, string fEnd)
    
    flags: Qt.Popup | Qt.NoDropShadowWindowHint

    visibility: Window.Hidden
    maximumHeight: Screen.height
    maximumWidth: Screen.width
    minimumHeight: Screen.height
    minimumWidth: Screen.width
    x: 0
    y: 0
    color: "transparent"
    
    Component.onCompleted: console.log("generate", maximumHeight, maximumWidth)
    //    onVisibleChanged: visibility = Window.Maximized
    S.AccountStyle{
        id: pageStyle
    }
    z: -50
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
        width: 200
        height: 150
        
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
                objectName: "generateButton"
                Layout.row: 2
                Layout.column: 0
                text: qsTr("Generate")
                onClicked: {
                    s_generate(from.text, to.text)
                    close();
                }
            }
            
            Button {
                Layout.row: 2
                Layout.column: 1
                text: qsTr("Cancel")
                onClicked: close()
            }
        }
    }
}
