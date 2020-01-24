import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4 as C1
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import "../Style"
import "../Core"

Popup {
    AccountStyle {
        id: pageStyle
    }
    
    CoreModel {
        id: models
    }
    
    id: root
    
    padding: 0
    
    contentItem: Rectangle {
        id: rectWindow
        anchors.fill: parent
        gradient: pageStyle.backgroundGradient
        
        property string budgetName
        border.color: "gold"
        
        GridLayout {
            id: gridId
            anchors.fill: parent
            anchors.leftMargin: root.width * 0.02
            anchors.rightMargin: root.width * 0.02
            anchors.topMargin: root.height * 0.02
            anchors.bottomMargin: root.height * 0.02
            
            columnSpacing: root.width * 0.02
            rowSpacing: root.height * 0.02
            
            Label {
                id: calendarLabel
                text: qsTr("From")
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                fontSizeMode: Text.Fit
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                
                Layout.preferredHeight: root.height * 0.25
                Layout.preferredWidth:  root.width * 0.47
                Layout.column: 0
                Layout.row: 0
                Layout.alignment: Qt.AlignLeft
            }
            
            CalendarButton {
                id: cButton
                objectName: "cButton"
                
                Layout.preferredHeight: root.height * 0.25
                Layout.preferredWidth:  root.width * 0.47
                Layout.column: 1
                Layout.row: 0
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignRight                
            }
            
            Label {
                text: qsTr("Target")
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                fontSizeMode: Text.Fit
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                
                Layout.preferredHeight: root.height * 0.25
                Layout.preferredWidth:  root.width * 0.47
                Layout.column: 0
                Layout.row: 1
                Layout.columnSpan: 1
                Layout.alignment: Qt.AlignLeft                
            }
            
            DoubleSpinBox {
                id: targetValue
                objectName: "targetValue"
                
                Layout.preferredHeight: root.height * 0.25
                Layout.preferredWidth:  root.width * 0.47
                Layout.column: 1
                Layout.row: 1
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignRight                
            }
            
            Label {
                text: qsTr("Every")
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                fontSizeMode: Text.Fit
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                
                Layout.preferredHeight: root.height * 0.25
                Layout.preferredWidth:  root.width * 0.47
                Layout.column: 0
                Layout.row: 2
                Layout.columnSpan: 1
                Layout.alignment: Qt.AlignLeft                
            }
            
            ComboBox {
                id: freqCombo
                objectName: "freqCombo"
                
                Layout.preferredHeight: root.height * 0.25
                Layout.preferredWidth:  root.width * 0.47
                Layout.column: 1
                Layout.row: 2
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignRight                
                
                model: models.freqModel
                textRole: "name"
                Rectangle {
                    gradient: pageStyle.goldButton
                    anchors.fill: parent
                }
                
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                
                property int currentRole: 0
                
                onCurrentIndexChanged: currentRole = models.freqModel.get(currentIndex).role
                
                delegate: ItemDelegate{
                    width: freqCombo.width
                    contentItem: Rectangle {
                        anchors.fill: parent
                        gradient: pageStyle.goldButton
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            acceptedButtons: Qt.NoButton
                        }
                        
                        Label {
                            anchors.centerIn: parent
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignHCenter
                            text: name
                            fontSizeMode: Text.Fit
                            font.family: pageStyle.core.name
                            font.pixelSize: pageStyle.core.size
                            
                        }
                    }
                }
            }
            
            Button {
                text: "ok"
                id: okButton
                objectName: "okButton"
                
                Layout.preferredHeight: root.height * 0.15
                Layout.preferredWidth:  root.width * 0.23
                Layout.column: 1
                Layout.row: 3
                Layout.columnSpan: 1
                Layout.alignment: Qt.AlignLeft                
                
                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }
            }
            
            Button {
                text: "Cancel"
                onClicked: root.close()
                
                Layout.preferredHeight: root.height * 0.15
                Layout.preferredWidth:  root.width * 0.22
                Layout.column: 2
                Layout.row: 3
                Layout.columnSpan:  1
                Layout.alignment: Qt.AlignRight                
                
                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }
            }
            
        }
    }
}

