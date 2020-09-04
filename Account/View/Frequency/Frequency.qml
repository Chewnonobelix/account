import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.13

import "../Core"
import "../Style"

Rectangle {    
    color: "transparent"
    border.color: "gold"
    
    ColumnLayout {
        anchors.fill: parent
        spacing: height * 0.02
        AccountHeader {
            text: qsTr("Frequency")
            
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
                        
            MouseArea {
                id: pastArea
                acceptedButtons: Qt.NoButton
                hoverEnabled: true
                anchors.fill: parent
            }
            
            ToolTip.text: qsTr("Display related transaction")
            ToolTip.timeout: 1000
            ToolTip.delay: 500
            ToolTip.visible: pastArea.containsMouse

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
                        _main.bind(modelData.id)
                    }
                }
                contentItem: AccountLabel {
                    text: Qt.formatDate(modelData.date, "dd-MM-yyyy")                   
                }
            }
            
            highlightMoveDuration: 0            
            highlight: Rectangle {
                gradient: AccountStyle.calSelect
            }
        }
        
        PageChanger {
            id: pageSwipper
            objectName: "pageSwipper"
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * 0.05
            onPageChange: _info.pageChange()
        }
    }
}
