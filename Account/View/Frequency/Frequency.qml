import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.13

import "../Core" as C
import "../Style" as S

Rectangle {    
    S.AccountStyle {
        id: pageStyle
    }
    
    color: "transparent"
    border.color: "gold"
    
    ColumnLayout {
        anchors.fill: parent
        spacing: height * 0.02
        Label {
            text: qsTr("Frequency")
            font.family: pageStyle.title.name
            font.pixelSize: pageStyle.title.size
            fontSizeMode: Text.Fit
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            background: Rectangle {
                gradient: pageStyle.goldHeader
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
                
                background: Rectangle {
                    color: "transparent"
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
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    
                   
                }
            }
            
            highlightMoveDuration: 0            
            highlight: Rectangle {
                gradient: pageStyle.calSelect
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
