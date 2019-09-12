import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "../Style"
import "../Core"

Window {
    AccountStyle {
        id: pageStyle
    }
    
    
    ListModel {
        id: frequencyModel
        objectName: "frequencyModel"
        ListElement {
            name: "freq1"
            idFreq: "3"
        }
    }
    
    Rectangle {
        anchors.fill: parent        
        gradient: pageStyle.backgroundGradient
        Column {
            
            id: colFreqList
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width * .3

            Label {
                text: qsTr("Frequency list")
            }
            
            ListView {
                id: frequencyList
                model: frequencyModel
                
                delegate: Text {
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                    fontSizeMode: Text.Fit
                    
                    text: name
                }
            }
            
        }
        
        Frequency {
            id: reference
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: colFreqList.right
            anchors.leftMargin: 10
            anchors.right: parent.right        
        }
    }    
}
