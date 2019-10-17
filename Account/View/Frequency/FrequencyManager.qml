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
                Layout.rowSpan: 2
                
                onCountChanged: currentIndex = count === 0 ? -1 : currentIndex
                clip: true
                Rectangle {
                    anchors.fill: parent
                    border.color: "gold"
                    color: "transparent"
                }
                
                
                onCurrentIndexChanged: ref.entry = model[currentIndex].reference
                Layout.alignment: Qt.AlignCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumWidth: parent.width * .20
                
                delegate: Rectangle {
                    height: 40
                    width: frequencyList.width
                    
                    gradient: index === frequencyList.currentIndex ? pageStyle.calSelect : pageStyle.unselectView
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            frequencyList.currentIndex = index
                        }
                        
                        cursorShape: Qt.PointingHandCursor
                    }
                    
                    Label {
                        anchors.fill: parent
                        font.family: pageStyle.core.name
                        font.pixelSize: pageStyle.core.size
                        fontSizeMode: Text.Fit
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: modelData.name === "" ? modelData.id : modelData.name
                    }
                }
            }
            
            Control2.Button {
                objectName: "addFreq"
                text: qsTr("+")
                
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                
                Layout.columnSpan:  1
                Layout.row: 4
                Layout.column: 0
                Layout.maximumWidth: parent.width * .10
                //                Layout.preferredHeight: parent.height * .05
                
                signal s_addFrequency()
                
                onClicked: s_addFrequency()
                
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
                objectName: "removeFreq"
                text: qsTr("-")
                
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                
                Layout.columnSpan:  1
                Layout.row: 4
                Layout.column: 1
                Layout.maximumWidth: parent.width * .10
                
                signal s_removeFrequency(int freq)
                
                onClicked: s_removeFrequency(frequencyList.model[frequencyList.currentIndex].id)
                
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
                //entry: frequencyList.model[frequencyList.currentIndex].reference
                Layout.preferredHeight: parent.height * .20
                Layout.preferredWidth: parent.width * .77
                Layout.columnSpan:  2
                Layout.rowSpan: 2
                Layout.row: 0
                Layout.column: 2
                enabled: frequencyList.count !== 0 
                property var incomeList: []
                property var outcomeList: []
                
                signal titleChanged(int id, string title)
                signal valueChanged(int id, real value)
                signal catChanged(int id, string cat)
                
                onS_valueChanged: if(enabled) valueChanged(frequencyList.model[frequencyList.currentIndex].id, value)
                onS_titleChanged: if(enabled) titleChanged(frequencyList.model[frequencyList.currentIndex].id, title)
                onS_catChanged: if(enabled) catChanged(frequencyList.model[frequencyList.currentIndex].id, cat, "manager")
                
                onEntryChanged: {
                    enabled = false
                    catModel = entry && entry.type === "income" ? incomeList : outcomeList
                    typeCombo.currentIndex = entry && entry.type === "income" ? 1 : 0
                    enabled = frequencyList.count !== 0 
                }
                
                onIncomeListChanged: catModel = entry && entry.type === "income" ? incomeList : outcomeList
                onOutcomeListChanged: catModel = entry && entry.type === "income" ? incomeList : outcomeList
            }
            
            ListView {
                id: entryList
                objectName: "entryList"
                //                                Layout.columnSpan:  1
                //                                Layout.rowSpan: 2
                Layout.row: 3
                Layout.column: 2
                Layout.alignment: Qt.Center
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: frequencyList.model[frequencyList.currentIndex].entries
                clip: true
                
                
                Rectangle {
                    anchors.fill: parent
                    border.color: "gold"
                    color: "transparent"
                }
                
                delegate: Rectangle {
                    color: "transparent"
                    height: 40
                    width: entryList.width                    
                    Label {
                        anchors.fill: parent
                        text: modelData
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                    }
                }
            }
            
            ListModel {
                id: typeModel
                ListElement {
                    name: qsTr("Outcome")
                    type: "outcome"
                }
                
                ListElement {
                    name: qsTr("Income")
                    type: "income"
                }
                
            }
            
            Control2.ComboBox {
                id: typeCombo
                objectName: "type"
                
                model: typeModel
                
                Layout.row: 2
                Layout.column: 2
                Layout.columnSpan: 1
                Layout.alignment: Qt.AlignTop | Qt.AlignRight
                Layout.preferredWidth: parent.width * 0.15    
                Layout.preferredHeight: parent.height * .07
                
                textRole: "name"
                
                background: Rectangle {
                    anchors.fill: parent
                    gradient: pageStyle.goldButton
                }

                signal s_updateType(int id, string nType)
                
                onCurrentIndexChanged: {
                    if(ref.enabled) s_updateType(ref.entry.id, model.get(currentIndex).type)
                }
                
                delegate: Control2.ItemDelegate {
                    width: typeCombo.width
//                    height: type.height
//                    text: name
                    contentItem: Rectangle {
                        anchors.fill: parent
                        gradient: pageStyle.goldButton
                        Label {
                            text: name
                            font.family: pageStyle.core.name
                            font.pixelSize: pageStyle.core.size
                            fontSizeMode: Text.Fit 
                            anchors.fill: parent
                            horizontalAlignment: Qt.AlignHCenter
                            verticalAlignment: Qt.AlignVCenter
                        }
                        MouseArea {
                            anchors.fill: parent
                            acceptedButtons: Qt.NoButton
                            cursorShape: Qt.PointingHandCursor
                        }
                    }
                }
            }
            
            
            Control2.Button {
                text: qsTr("Generate")
                
                Layout.row: 2
                Layout.column: 3
                Layout.columnSpan: 1
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.maximumWidth: parent.width * 0.15            
                Layout.preferredHeight: parent.height * .07
                
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
                
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
            }        
        }
    }/**/
    
    
}
