import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4 as C1
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import "../Style"
import "../Core"

Window {
    flags: Qt.Popup | Qt.NoDropShadowWindowHint
    
    x: screen.width / 2 - width / 2
    y: screen.height / 2 - height / 2
    
    minimumHeight: 350
    minimumWidth: 350
    
    AccountStyle {
        id: pageStyle
    }
    
    CoreModel {
        id: models
    }

    width: rectWindow.width * 2
    //    height: (col2.height)* 1.10
    id: main
    color: "transparent"
    Rectangle {
        id: rectWindow
        anchors.left: parent.left
        anchors.top: parent.top

        height:  (20*3 + calendarLabel.height * 5) * 1.15
        width: (calendarLabel.width + targetValue.width)*1.3
        gradient: pageStyle.backgroundGradient
        
        property string budgetName
        border.color: "gold"
        
        Grid {
            id: gridId
            columns: 2
            columnSpacing: 10
            rowSpacing: 20
            
            
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.topMargin: 10
            horizontalItemAlignment: Qt.AlignHCenter
            verticalItemAlignment: Qt.AlignVCenter
            
            Label {
                id: calendarLabel
                text: qsTr("From")
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                fontSizeMode: Text.Fit
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
            }
            CalendarButton {
                id: cButton
                objectName: "cButton"
                
            }
            
            Label {
                text: qsTr("Target")
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                fontSizeMode: Text.Fit
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
            }
            
            DoubleSpinBox {
                id: targetValue
                objectName: "targetValue"
            }
            
            Label {
                text: qsTr("Every")
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                fontSizeMode: Text.Fit
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
            }
            
            
            
            ComboBox {
                id: freqCombo
                objectName: "freqCombo"

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
                
                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }
            }
            
            Button {
                text: "Cancel"
                onClicked: main.close()
                
                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }
                
            }
            
        }
    }
}

