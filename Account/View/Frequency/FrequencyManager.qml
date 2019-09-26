import QtQuick 2.12
import QtQuick.Controls 2.5 as Control2
import QtQuick.Window 2.13
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.13

import "../Style"
import "../Core"

Window {
    AccountStyle {
        id: pageStyle
    }
    title: qsTr("Frequency manager")
    
    visible: false
    Rectangle {
        anchors.fill: parent        
        gradient: pageStyle.backgroundGradient
        ColumnLayout {
            id: colFreqList
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.topMargin: 10
            width: parent.width * .15
            
            Label {
                text: qsTr("Frequency list")
                Layout.maximumHeight: parent.height * .10
                Layout.fillWidth: true
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter         
            }
            
            
            ListView {                
                id: frequencyList
                objectName: "frequencyList"
                model: []
                anchors.leftMargin: 10
                
                clip: true
                Rectangle {
                    anchors.fill: parent
                    border.color: "gold"
                    color: "transparent"
                }
                
                Layout.fillHeight: true
                Layout.fillWidth: true
                height: parent.height * .8
                width: parent.width
                section.property:  "name"
                delegate: Label {
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                    fontSizeMode: Text.Fit
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    
                    height: 40
                    width: frequencyList.width
                    text: modelData.name + "//" + modelData.id
                    Component.onCompleted: console.log("Del ", text, index, modelData.name)
                }
            }
            
            RowLayout {
                Layout.fillWidth: true
                Layout.maximumHeight: parent.height * 0.03
                spacing: 5               
                Control2.Button {
                    Layout.preferredWidth: parent * 0.5
                    Layout.fillHeight: true
                    
                    text: qsTr("+")
                    onClicked: console.log(text)
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                    
                    MouseArea {
                        z: -1
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        acceptedButtons: Qt.NoButton
                    }
                    
                    Rectangle {
                        anchors.fill: parent
                        gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton 
                    }
                }
                
                Control2.Button {
                    Layout.preferredWidth: parent * 0.5
                    Layout.fillHeight: true
                    
                    text: qsTr("-")
                    onClicked: console.log(text)
                    
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                    
                    MouseArea {
                        z: -1
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        acceptedButtons: Qt.NoButton
                    }
                    
                    Rectangle {
                        anchors.fill: parent
                        gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton 
                    }
                    
                }
            }            
        }
        
        //        Frequency {
        //            id: reference
        //            anchors.top: parent.top
        //            anchors.bottom: parent.bottom
        //            anchors.left: colFreqList.right
        //            anchors.leftMargin: 10
        //            anchors.right: parent.right        
        //        }
    }    
}/**/