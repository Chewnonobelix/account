import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.13

import "../Core" as C
import "../Style"

Rectangle {    
    color: "transparent"
    border.color: "gold"
    
    ColumnLayout {
        anchors.fill: parent
        spacing: height * 0.02
        Label {
            text: qsTr("Frequency")
            font.family: AccountStyle.title.name
            font.pixelSize: AccountStyle.title.size
            fontSizeMode: Text.Fit
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            background: Rectangle {
                gradient: AccountStyle.goldHeader
            }
            
            Layout.preferredHeight: parent.height * 0.05
            Layout.fillWidth: true
        }
        
        ListView {
            id: pastView
            objectName: "frequencyPast"
            model: []
            clip: true
            Layout.fillHeight: true
            Layout.fillWidth: true
            
            signal s_showFromFrequency(int id)
            
            delegate: ItemDelegate {
                highlighted: ListView.isCurrentItem
                
                width: ListView.view.width
                height: ListView.view.height * 0.07
                
                background: AccountBackground {
                    invisible: true
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked:  {
                        pastView.currentIndex = index
                        pastView.s_showFromFrequency(modelData.id)
                    }
                }
                contentItem: Label {
                    text: Qt.formatDate(modelData.date, "dd-MM-yyyy")
                    fontSizeMode: Text.Fit
                    font.family: AccountStyle.core.name
                    font.pixelSize: AccountStyle.core.size
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    
                   
                }
            }
            
            highlightMoveDuration: 0            
            highlight: Rectangle {
                gradient: AccountStyle.calSelect
            }
        }
        
        C.PageChanger {
            id: pageSwipper
            objectName: "pageSwipper"
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.05
            
        }
    }
}
