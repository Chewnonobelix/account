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
    
    maximumHeight: Screen.height / 2
    maximumWidth: Screen.width * 0.4
    visible: false
    
    
    width: maximumWidth
    height: maximumHeight
    Rectangle {
        anchors.fill: parent
        gradient: pageStyle.backgroundGradient
        GridLayout {
            
            anchors.fill: parent
            Rectangle {
                gradient: pageStyle.goldHeader
                
                Label {
                    anchors.fill: parent
                    text: qsTr("Reference") + " ->"
                    font.family: pageStyle.title.name
                    font.pixelSize: pageStyle.title.size
                    fontSizeMode: Text.Fit
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                
                
                Layout.alignment: Qt.AlignCenter
                Layout.columnSpan:  2
                Layout.row: 0
                Layout.column: 0
                Layout.preferredWidth: parent.width * .20
                Layout.preferredHeight: parent.height * .05
            }
            
            
            Rectangle {
                gradient: pageStyle.goldHeader
                
                Label {
                    anchors.fill: parent
                    text: qsTr("Frequency list")
                    font.family: pageStyle.title.name
                    font.pixelSize: pageStyle.title.size
                    fontSizeMode: Text.Fit
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                
                
                    Layout.alignment: Qt.AlignCenter
                    Layout.columnSpan:  2
                    Layout.row: 1
                    Layout.column: 0
                    Layout.preferredWidth: parent.width * .20
                    Layout.preferredHeight: parent.height * .05
            }
            
            ListView {
                id: frequencyList
                objectName: "frequencyList"
                model: []
                anchors.leftMargin: 10
                Layout.columnSpan:  2
                Layout.row: 2
                Layout.column: 0
                
                clip: true
                Rectangle {
                    anchors.fill: parent
                    border.color: "gold"
                    color: "transparent"
                }
                
                
                Layout.alignment: Qt.AlignCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumWidth: parent.width * .20
                section.property:  "id"
                section.labelPositioning: ViewSection.CurrentLabelAtStart
                section.delegate: Label {
                    height: 40
                    width: frequencyList.width
                    text: modelData
                }

                delegate: Rectangle {
                    height: 40
                    width: frequencyList.width
                    
                    gradient: index === frequencyList.currentIndex ? pageStyle.calSelect : pageStyle.unselectView
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: frequencyList.currentIndex = index
                        
                        cursorShape: Qt.PointingHandCursor
                    }
                    
                    Label {
                        anchors.fill: parent
                        font.family: pageStyle.core.name
                        font.pixelSize: pageStyle.core.size
                        fontSizeMode: Text.Fit
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: modelData.name
                    }
                }
            }
            
            Control2.Button {
                
                text: qsTr("+")
                onClicked: console.log(text)
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                
                Layout.columnSpan:  1
                Layout.row: 3
                Layout.column: 0
                Layout.maximumWidth: parent.width * .10
                //                Layout.preferredHeight: parent.height * .05
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
                
                text: qsTr("-")
                onClicked: console.log(text)
                
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                
                Layout.columnSpan:  1
                Layout.row: 3
                Layout.column: 1
                Layout.maximumWidth: parent.width * .10
                
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
            
            
            EntryEdit {
                id: ref
                objectName: "ref"
                
                Layout.preferredHeight: parent.height * .20
                Layout.preferredWidth: parent.width * .75
                Layout.columnSpan:  1
                Layout.rowSpan: 2
                Layout.row: 0
                Layout.column: 2
                
                onWidthChanged: console.log(width)
            }
            
            ListView {
                id: entryList
                objectName: "entryList"
//                                Layout.columnSpan:  1
//                                Layout.rowSpan: 2
                                Layout.row: 2
                                Layout.column: 2
                
                
                model: ["1", "2", "3"]
                Rectangle {
                    anchors.fill: parent
                    border.color: "gold"
                    color: "transparent"
                }
                
                delegate: Rectangle {
                    color: "transparent"
                    Label {
                        height: 40
                        anchors.fill: parent
                        text: modelData
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
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
