import QtQuick 2.13
import QtQuick.Controls 2.5
import QtQuick.Window 2.12
import QtQuick.Layouts 1.13
import "../Style" as S
import "../Core" as C

Popup {
    property int freqId: -1
    property int freqGroup: -1
    
    signal s_generate(string fBegin, string fEnd)
    
    id: generateWin
    
    property var pcenter
    
    S.AccountStyle{
        id: pageStyle
    }
    
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.NoButton
        propagateComposedEvents: true
        preventStealing: false
        hoverEnabled: true
    }
    contentWidth: l.width
    contentHeight: l.height
    leftPadding: l.width * 0.05
    rightPadding: l.width * 0.05
    topPadding: l.height * 0.05
    bottomPadding: l.height * 0.05
    
    background: Rectangle {
        gradient: pageStyle.backgroundGradient
        border.color: "gold"        
    }

    contentItem: Rectangle {
        id: visbleRect
        gradient: pageStyle.unselectView

        anchors.centerIn: parent
        clip: false     
        GridLayout {
            id: l
            clip: false
            
            Label {
                Layout.row: 0
                Layout.column: 0
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size2
                fontSizeMode: Text.Fit
                text: qsTr("From")
            }
            
            C.CalendarButton {
                Layout.row: 1
                Layout.column: 0
                id: from
                clip: false
                onTextChanged: {
                    parent.checkDate()
                }
                
            }
            
            Label {
                Layout.row: 0
                Layout.column: 1
                text:qsTr("To")
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size2
                fontSizeMode: Text.Fit
            }
            
            C.CalendarButton {
                Layout.row: 1
                Layout.column: 1
                id: to
                
                onTextChanged: {
                    parent.checkDate()
                }
            }
            
            function checkDate() {
                var t = Date.fromLocaleDateString(Qt.locale(), to.text, "dd-MM-yyyy")
                var f = Date.fromLocaleDateString(Qt.locale(),from.text, "dd-MM-yyyy")
                
                if(t < f ||  to.text === "" ) {
                    to.extern(f)
                } 
            }
            
            Button {
                objectName: "generateButton"
                Layout.row: 2
                Layout.column: 0
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                
                text: qsTr("Generate")
                onClicked: {
                    s_generate(from.text, to.text)
                    close();
                }
                
                background: Rectangle {
                    gradient: pageStyle.goldButton
                }
            }
            
            Button {
                Layout.row: 2
                Layout.column: 1
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                
                text: qsTr("Cancel")
                onClicked: close()
                
                
                background: Rectangle {
                    gradient: pageStyle.goldButton
                }
            }
        }
    }
}
