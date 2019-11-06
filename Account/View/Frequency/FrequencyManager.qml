import QtQuick 2.12
import QtQuick.Controls 2.13 as Control2
import QtQuick.Window 2.13
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.13

import Account 1.0

import "../Style"
import "../Core"

Window {
    AccountStyle {
        id: pageStyle
    }
    
    CoreModel {
        id: models
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
                
                onCountChanged: {           
                    currentIndex = count === 0 ? -1 : currentIndex
                    ref.enabled = count !== 0
                }
                
                clip: true
                Rectangle {
                    anchors.fill: parent
                    border.color: "gold"
                    color: "transparent"
                }
                
                
                onCurrentIndexChanged: {
                    if(enabled) {
                        whenCombo.enabled = false
                        ref.entry = model[currentIndex].reference
                        var t = model[currentIndex].freq + 0
                        whenCombo.currentIndex = whenCombo.model.findIndex(t)
                        
                        testModel.clear()
                        for(var i = 0; i < model[currentIndex].listEntries().length; i++) {
                            testModel.append(model[currentIndex].listEntries()[i])
                        }
                        
                        groupText.nb = model[currentIndex].nbGroup
                        countText.nb = model[currentIndex].listEntries().length
                        whenCombo.enabled = count !== 0                    
                    }
                }
                Layout.alignment: Qt.AlignCenter
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.maximumWidth: parent.width * .20
                
                delegate: Rectangle {
                    height: 40
                    width: frequencyList.width
                    
                    gradient: ListView.isCurrentItem ? ref.entry.type === "income" ? pageStyle.selectViewIn : pageStyle.selectViewOut : pageStyle.unselectView
                    
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
                Layout.preferredHeight: parent.height * .20
                Layout.preferredWidth: parent.width * .77
                Layout.columnSpan:  2
                Layout.rowSpan: 2
                Layout.row: 0
                Layout.column: 2
                enabled: frequencyList.count !== 0  && whenCombo.enabled
                property var incomeList: []
                property var outcomeList: []
                
                signal titleChanged(int id, string title)
                signal valueChanged(int id, real value)
                signal catChanged(int id, string cat)
                
                onS_valueChanged: if(entry && enabled) valueChanged(entry.id, value)
                onS_titleChanged: if(entry && enabled) titleChanged(entry.id, title)
                onS_catChanged: if(entry && enabled) catChanged(entry.id, cat, "manager")
                
                onEntryChanged: {
                    catModel = entry && entry.type === "income" ? incomeList : outcomeList
                    typeCombo.currentIndex = entry && entry.type === "income" ? 0 : 1
                    reloadCat()
                }
                
                onIncomeListChanged: catModel = entry && entry.type === "income" ? incomeList : outcomeList
                onOutcomeListChanged: catModel = entry && entry.type === "income" ? incomeList : outcomeList
            }
            
            ListModel {
                id: testModel
            }
            
            Column {
                Layout.row: 3
                Layout.column: 3
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: ref.width / 2
                
                Rectangle {
                    width: parent.width
                    height: parent.height * .20
                    color: "transparent"
                    border.color: "gold"
                    Text {
                        id: groupText
                        property int nb: 0
                        text: qsTr(" Number of generation") + ": " + nb 
                        fontSizeMode: Text.Fit
                        font.family: pageStyle.core.name
                        font.pixelSize: pageStyle.core.size
                        width: parent.width
                        height: parent.height * 0.50                    
                    }                
                    Text {
                        id: countText
                        anchors.top: groupText.bottom
                        
                        property int nb: 0
                        text: qsTr(" Number of entries") + ": " + nb 
                        fontSizeMode: Text.Fit
                        font.family: pageStyle.core.name
                        font.pixelSize: pageStyle.core.size
                        width: parent.width
                        height: parent.height * 0.50                    
                    }                
                }
                
                Rectangle {
                    objectName: "linkedDisplayer"
                    width: parent.width
                    height: parent.height * .80
                    color: "transparent"
                    property var entry
                    onEntryChanged: {   
                        linked.entry = entry
                    }
                    
                    EntryEdit {
                        id: linked
                        anchors.fill: parent
                        Component.onCompleted: changeDirection()
                        enabled: false
                    }
                }                
            }
            
            ListView {
                id: entryList
                objectName: "entryList"
                Layout.row: 3
                Layout.column: 2
                Layout.alignment: Qt.Center
                //                Layout.fillWidth: true
                Layout.fillHeight: true
                
                Layout.preferredWidth: ref.width / 2
                model: testModel
                clip: true
                
                signal s_display(int entryId)
                
                property var enabledSection: []
                section.property: "group" 
                section.criteria: ViewSection.FullString
                section.labelPositioning: ViewSection.InlineLabels
                section.delegate: Rectangle {
                    gradient: pageStyle.goldHeader
                    width: ListView.view.width
                    height: ListView.view.height * 0.10
                    Text {
                        anchors.fill: parent
                        text: section
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        fontSizeMode: Text.Fit
                        font.family: pageStyle.title.name
                        font.pixelSize: pageStyle.title.size2
                    }
                    
                    Text {
                        id: arrow
                        anchors.right: parent.right
                        anchors.rightMargin: parent.width * 0.05
                        height: parent.height
                        text: ">"
                        rotation: 90
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        fontSizeMode: Text.Fit
                        font.family: pageStyle.title.name
                        font.pixelSize: pageStyle.title.size2                        
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            entryList.enabledSection[section] = !entryList.enabledSection[section]  
                            
                            for(var j = 0; j < testModel.count; j++) {
                                if(testModel.get(j).group == section ) testModel.setProperty(j, "isVisible", entryList.enabledSection[section]);
                            }
                            
                            arrow.rotation = arrow.rotation + 180 
                        }
                    }
                    
                    Component.onCompleted: entryList.enabledSection[section] = true
                }
                Rectangle {
                    anchors.fill: parent
                    border.color: "gold"
                    color: "transparent"
                }
                
                delegate: Control2.ItemDelegate {
                    objectName: "entryDel"
                    background: Rectangle {color: "transparent" }
                    width: ListView.view.width
                    height: isVisible ? ListView.view.height * 0.07 : 0
                    visible: isVisible
                    
                           
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            entryList.s_display(id)
                        }
                    }
                    
                    Label {                        
                        anchors.fill: parent
                        text: date
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        fontSizeMode: Text.Fit
                        font.family: pageStyle.core.name
                        font.pixelSize: pageStyle.core.size
                    }
                }
            }
            
            
            Row {
                Layout.row: 2
                Layout.column: 2
                Layout.columnSpan: 3
                spacing: width * 0.01
                Layout.fillWidth: true    
                Layout.preferredHeight: parent.height * .07
                
                
                Control2.ComboBox { 
                    id: whenCombo
                    objectName: "whenCombo"
                    height: parent.height
                    width: parent.width / parent.children.length
                    model: models.freqModel
                    textRole: "name"
                    signal s_freq(int i, int f)
                    
                    onCurrentIndexChanged: {
                        if(enabled && ref.entry) s_freq(ref.entry.id, model.get(currentIndex).role)
                    }
                    
                    background: Rectangle {
                        anchors.fill: parent
                        gradient: pageStyle.goldButton
                    }
                    
                    delegate: Control2.ItemDelegate {
                        width: typeCombo.width
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
                
                Control2.ComboBox {
                    id: typeCombo
                    objectName: "type"
                    
                    model: models.typeModel
                    enabled: whenCombo.enabled
                    height: parent.height
                    width: parent.width / parent.children.length
                    
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
                    objectName: "generateButton"
                    text: qsTr("Generate")
                    
                    //                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    //                    Layout.maximumWidth: parent.width * 0.15            
                    //                    Layout.preferredHeight: parent.height * .07
                    
                    height: parent.height
                    width: parent.width / parent.children.len
                    
                    signal s_open(int fId)
                    
                    onReleased: s_open(ref.entry.id)
                    
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
        }
    }/**/
    
    
}
