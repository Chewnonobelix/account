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
    
    width: rectWindow.width * 2 
    //    height: (col2.height)* 1.10
    id: main
    color: "transparent"
    Rectangle {
        id: rectWindow
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
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
                //            anchors.fill: parent
                text: qsTr("From")
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                fontSizeMode: Text.Fit
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                
                background: Rectangle {
                    border.color: "gold"
                }
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
            
            ListModel {
                id: freqModel
                ListElement {
                    name: qsTr("Day")
                    role: 1
                }
                ListElement {
                    name: qsTr("Week")
                    role: 2
                }
                ListElement {
                    name: qsTr("Month")
                    role: 3
                }
                ListElement {
                    name: qsTr("Quarter")
                    role: 4
                }
                ListElement {
                    name: qsTr("Year")
                    role: 5
                }                
            }
            
            ComboBox {
                id: freqCombo
                model: freqModel
                textRole: "name"
                Rectangle {
                    gradient: pageStyle.goldButton
                    anchors.fill: parent
                }

                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                
                onCurrentTextChanged: console.log(currentIndex, currentText)
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
    
    //    Rectangle {
    //        width: (( col2.x + col2.width) - col1.x) * 1.05
    //        height: (( row1.y + row1.height) - col2.y) * 1.05   
    
    //        anchors.leftMargin: 10
    //        anchors.topMargin: 10
    //        border.color: "gold"
    //        gradient: pageStyle.backgroundGradient
    //        ColumnLayout {
    //            id: col1
    ////            topPadding: 10
    ////            rightPadding: 10
    ////            leftPadding: 10
    
    //            Rectangle {
    //                Layout.fillWidth: true
    //                Layout.fillHeight: true
    //                color: "transparent"
    //                border.color: "gold"
    //            }
    
    //            spacing: 10             
    //            Label {
    //                Layout.alignment: Qt.AlignCenter
    //                anchors.topMargin: 10
    //                anchors.leftMargin: 10
    
    //                id: l_reference
    //                text: qsTr("Reference Date")
    //                fontSizeMode: Text.Fit
    //                font.family: pageStyle.title.name
    //                font.pixelSize: pageStyle.title.size
    
    //            }
    
    //            CalendarButton {
    //                id: cButton
    //                objectName: "cButton"
    //                Layout.alignment: Qt.AlignCenter
    //            }
    //        }
    
    //        Rectangle {
    //            Layout.fillWidth: true
    //            Layout.fillHeight: true
    //            color: "transparent"
    //            border.color: "gold"
    
    //        ColumnLayout {
    //            id: col2
    ////            anchors.topMargin: 10        
    //            anchors.left: col1.right
    //            anchors.leftMargin: 10
    //            anchors.verticalCenter: col1.verticalCenter
    
    ////            topPadding: 10
    ////            rightPadding: 10
    ////            leftPadding: 10
    //            spacing: 10             
    
    
    //            Label {
    //                anchors.topMargin: 10
    //                anchors.leftMargin: 10
    
    //                id: l_frequency
    //                text: qsTr("Target")
    //                fontSizeMode: Text.Fit
    //                font.family: pageStyle.title.name
    //                font.pixelSize: pageStyle.title.size
    
    //                Layout.alignment: Qt.AlignCenter
    //            }
    
    //            DoubleSpinBox {
    //                id: targetValue
    //                objectName: "targetValue"                
    //                Layout.alignment: Qt.AlignCenter
    //            }
    //        }
    //        }
    
    //        Row {
    //            id: row1
    //            anchors.top: col2.bottom
    //            anchors.topMargin: 10
    //            anchors.rightMargin: 10
    //            anchors.right: col2.right
    //            spacing: 10
    //            Button {
    //                text: "ok"
    //                objectName: "okButton"
    
    //                background: Rectangle {
    //                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
    //                }
    //            }
    
    //            Button {            
    //                text: "Cancel"
    //                onClicked: main.close()
    
    //                background: Rectangle {
    //                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
    //                }
    
    //            }
    //        }
    //    }
}

