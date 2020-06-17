import QtQuick 2.12
import QtQuick.Controls 2.13 as Control2
import QtQuick.Window 2.13
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.13

import Account 1.0

import "../Style"
import "../Core"

import Account.Frequency 1.0

Rectangle {    
    ListModel {
        id: testModel
    }
    
    onFocusChanged: s_open()
    
    signal s_open()
    
    color: "transparent"
    
    id: root
    
    Component.onCompleted: {
        layout.usableHeight = Qt.binding(function() {return height})
        layout.usableWidth = Qt.binding(function() {return width})      
    }
    
    
    Rectangle {
        anchors.fill: parent
        gradient: AccountStyle.unselectView
        
        Generate {
            objectName: "generate"
            anchors.centerIn: rect
        }
        
        id: rect
        GridLayout {
            anchors.fill: parent
            anchors.topMargin: usableHeight * 0.02
            anchors.bottomMargin: usableHeight * 0.02
            anchors.rightMargin: usableWidth * 0.02
            anchors.leftMargin: usableWidth * 0.02
            id: layout
            
            property int usableHeight
            property int usableWidth
            
            rowSpacing: usableHeight * 0.02
            columnSpacing: usableWidth * 0.02
            
            AccountHeader {
                text: qsTr("Reference") + " ->"
                
                Layout.preferredHeight: parent.usableHeight * 0.05
                Layout.preferredWidth: parent.usableWidth * 0.20
                Layout.row: 0
                Layout.column: 0
                Layout.columnSpan: 2
                Layout.rowSpan: 1
                Layout.alignment: Qt.AlignTop
            }
            
            ListView {
                id: frequencyList
                objectName: "frequencyList"
                model: []
                
                Layout.preferredHeight: parent.usableHeight * 0.77
                Layout.preferredWidth: parent.usableWidth * 0.20
                Layout.row: 1
                Layout.column: 0
                Layout.columnSpan: 2
                Layout.rowSpan: 3
                currentIndex: -1
                header:  AccountHeader {
                    text: qsTr("Frequency list")
                    height: frequencyList.height * .10
                    width: frequencyList.width
                }
                
                property var currentModel: null
                
                signal s_modelChanged(string name)
                onCurrentModelChanged: {
                    if(currentModel) s_modelChanged(currentModel.name)
                }

                Component.onCompleted: {
                    ref.entry = Qt.binding(function() {return currentModel ? currentModel.reference : null})
                    groupText.nb = Qt.binding(function() {return currentModel ? currentModel.nbGroup : 0})
                    countText.nb = Qt.binding(function() {return currentModel ? currentModel.count : 0})
                    pageChanger.maxPage = Qt.binding(function() {return countText.nb / 100 + 1})
                    whenCombo.enabled = Qt.binding(function() {return count !== 0})
                    whenCombo.currentIndex = Qt.binding(function() {return currentModel  ? whenCombo.model.findIndex(currentModel.freq + 0) : 0})
                    endless.checked = Qt.binding(function() {return currentModel ? currentModel.endless : false})
                }
                
                clip: true
                Rectangle {
                    anchors.fill: parent
                    border.color: "gold"
                    color: "transparent"
                }
                
                onModelChanged: {
                    currentIndex = -1
                }
                
                onCurrentIndexChanged: {
                    if(enabled) {
                        pageChanger.pageIndex = 1                                                                        
                        pageChanger.s_pageChange()
                        currentModel = enabled && currentIndex > -1 ? model[currentIndex] : null
                    }
                }
                
                delegate: Rectangle {
                    height: 40
                    width: frequencyList.width
                    
                    gradient: ref.entry && ListView.isCurrentItem ? ref.entry.type === "income" ? AccountStyle.selectViewIn : AccountStyle.selectViewOut : AccountStyle.unselectView
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            frequencyList.currentIndex = index
                        }
                        
                        cursorShape: Qt.PointingHandCursor
                    }
                    
                    AccountLabel {
                        anchors.fill: parent
                        text: modelData.name === "" ? modelData.id : modelData.name
                    }
                }
            }
            
            AccountButton {
                objectName: "addFreq"
                text: qsTr("+")
                
                Layout.preferredHeight: parent.usableHeight * 0.05
                Layout.preferredWidth: parent.usableWidth * 0.09
                Layout.row: 4
                Layout.column: 0
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                
                signal s_addFrequency()
                
                onClicked: s_addFrequency()
            }
            
            AccountButton {
                objectName: "removeFreq"
                text: qsTr("-")
                
                Layout.preferredHeight: parent.usableHeight * 0.05
                Layout.preferredWidth: parent.usableWidth * 0.09
                Layout.row: 4
                Layout.column: 1
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                
                signal s_removeFrequency(int freq)
                
                onClicked: s_removeFrequency(frequencyList.currentModel.id)
            }
            
            EntryEdit {
                id: ref
                objectName: "ref"
                
                Layout.preferredHeight: parent.usableHeight * 0.10
                Layout.preferredWidth: parent.usableWidth * 0.74
                Layout.row: 0
                Layout.column: 2
                Layout.columnSpan: 3
                Layout.rowSpan: 1
                
                
                enabled: frequencyList.count !== 0  && whenCombo.enabled
                property var incomeList: []
                property var outcomeList: []
                
                signal titleChanged(int id, string title)
                signal valueChanged(int id, real value)
                signal catChanged(int id, string cat)
                
                onS_valueChanged: if(entry && enabled) valueChanged(frequencyList.currentModel.id, value)
                onS_titleChanged: if(entry && enabled) titleChanged(frequencyList.currentModel.id, title)
                onS_catChanged: if(entry && enabled) catChanged(frequencyList.currentModel.id, cat, "manager")
                
                onEntryChanged: {
                    typeCombo.currentIndex = CoreModel.typeModel.find(entry ? entry.type: "outcome")
                }
                
                Component.onCompleted: {
                    enabled = Qt.binding(function() {return frequencyList.count !== 0})
                    
                    catModel = Qt.binding(function(){return (entry && entry.type === "income") ? incomeList : outcomeList})
                    linked.catModel = Qt.binding(function() { return catModel})
                }
            }
            
            AccountComboBox {
                id: whenCombo
                objectName: "whenCombo"
                
                Layout.preferredHeight: parent.usableHeight * 0.05
                Layout.preferredWidth: parent.usableWidth * 0.23
                Layout.row: 1
                Layout.column: 2
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                
                enabled: ref.enabled
                
                
                model: CoreModel.freqModel
                textRole: "name"
                signal s_freq(int i, int f)
                
                onCurrentIndexChanged: {
                    if(down && ref.entry) s_freq(frequencyList.currentModel.id, model.get(currentIndex).role)
                }                
            }
            
            AccountComboBox {
                id: typeCombo
                objectName: "type"
                
                model: CoreModel.typeModel
                
                Layout.preferredHeight: parent.usableHeight * 0.05
                Layout.preferredWidth: parent.usableWidth * 0.24
                Layout.row: 1
                Layout.column: 3
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                
                enabled: ref.enabled
                
                
                textRole: "name"
                valueRole: "type"
                
                signal s_updateType(int id, string nType)
                
                onCurrentIndexChanged: {
                    if(ref.entry && down){ 
                        s_updateType(frequencyList.currentModel.id, model.get(currentIndex).type)
                    }
                }
            }
            
            AccountBackground {
                invisible: true
                Layout.preferredHeight: parent.usableHeight * 0.05
                Layout.preferredWidth: parent.usableWidth * 0.23
                Layout.row: 1
                Layout.column: 4
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                
                AccountCheckBox {
                    id: endless
                    objectName: "endless"
                    anchors.left: parent.left
                    text: qsTr("Endless")
                    signal s_endless(int id, bool e)
                    
                    onCheckedChanged: if(frequencyList.currentModel) s_endless(frequencyList.currentModel.id, checked)
                }

                AccountButton {
                    objectName: "generateOpen"
                    text: qsTr("Generate")
                    
                    anchors.right: parent.right
                    anchors.left: endless.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    enabled: ref.enabled && !endless.checked
                    
                    signal s_open(int fId)
                    
                    onReleased: s_open(frequencyList.currentModel.id)
                }
                
            }

            
            Rectangle {
                Layout.preferredHeight: parent.usableHeight * 0.20
                Layout.preferredWidth: parent.usableWidth * 0.49
                Layout.row: 2
                Layout.column: 3
                Layout.columnSpan: 2
                Layout.rowSpan: 1
                
                visible: frequencyList.currentIndex !== -1
                color: "transparent"
                border.color: "gold"
                
                AccountLabel {
                    id: groupText
                    property int nb: 0
                    text: qsTr("Number of generation") + ": " + nb
                    width: parent.width
                    height: parent.height / parent.children.length
                }
                
                AccountLabel {
                    id: countText
                    anchors.top: groupText.bottom
                    
                    property int nb: 0
                    text: qsTr("Number of entries") + ": " + nb
                    width: parent.width
                    height: parent.height / parent.children.length
                }
                
                AccountLabel {
                    id: dateText
                    property var from: frequencyList.currentModel ? frequencyList.currentModel.begin: ""
                    property var to: frequencyList.currentModel ? frequencyList.currentModel.end: ""
                    anchors.top: countText.bottom
                    text: qsTr("From") + " " + Qt.formatDate(from, "dd-MM-yyyy") + ", " + qsTr("to") + " " + Qt.formatDate(to, "dd-MM-yyyy")
                    width: parent.width
                    height: parent.height / parent.children.length
                }

                AccountLabel {
                    objectName: "worker"
                    anchors.top: dateText.bottom
                    width: parent.width
                    height: parent.height / parent.children.length

                    property Worker worker: null;
                    text: qsTr("Progress") + ": "  + (worker ? worker.progress + "%" : "N/A")
                }
            }
            
            EntryEdit {
                id: linked
                objectName: "linkedDisplayer"
                
                Component.onCompleted: changeDirection()
                opening: true
                visible: entryList.currentIndex !== -1 && frequencyList.currentIndex !== -1
                
                Layout.preferredHeight: parent.usableHeight * 0.53
                Layout.preferredWidth: parent.usableWidth * 0.49
                Layout.row: 3
                Layout.column: 3
                Layout.columnSpan: 2
                Layout.rowSpan: 2
                
            }
            
            ListView {
                id: entryList
                objectName: "entryList"
                
                Layout.preferredHeight: parent.usableHeight * 0.70
                Layout.preferredWidth: parent.usableWidth * 0.23
                Layout.row: 2
                Layout.column: 2
                Layout.columnSpan: 1
                Layout.rowSpan: 2
                
                model: testModel
                clip: true
                
                signal s_display(int entryId)
                
                property var enabledSection: []
                section.property: "group"
                section.criteria: ViewSection.FullString
                section.labelPositioning: ViewSection.InlineLabels
                section.delegate: Rectangle {
                    gradient: AccountStyle.goldHeader
                    width: ListView.view.width
                    height: ListView.view.height * 0.10
                    Text {
                        anchors.fill: parent
                        text: section
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        fontSizeMode: Text.Fit
                        font.family: AccountStyle.title.name
                        font.pixelSize: AccountStyle.title.size2
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
                        font.family: AccountStyle.title.name
                        font.pixelSize: AccountStyle.title.size2
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
                
                highlight: Rectangle {
                    gradient: AccountStyle.calSelect
                }
                
                highlightMoveDuration: 0
                delegate: Control2.ItemDelegate {
                    objectName: "entryDel"
                    background: AccountBackground {
                        property var index: parent.index
                        invisible: true
                    }
                    
                    highlighted: ListView.isCurrentItem
                    width: ListView.view.width
                    height: isVisible ? ListView.view.height * 0.07 : 0
                    visible: isVisible
                    
                    function f() {
                        entryList.s_display(id)
                        ListView.view.currentIndex = index
                    }
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            parent.f()
                        }
                    }
                    
                    AccountLabel {
                        anchors.fill: parent
                        text: Qt.formatDate(date, "dd-MM-yyyy")
                    }
                }
            }
            
            PageChanger {
                id: pageChanger
                
                Layout.preferredHeight: parent.usableHeight * 0.05
                Layout.preferredWidth: parent.usableWidth * 0.23
                Layout.row: 4
                Layout.column: 2
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                
                onS_pageChange: {
                    testModel.clear()
                    
                    var i = pageIndex - 1
                    if(frequencyList.currentModel) {
                        for(var j = 0 ; j < 100; j++) {
                            if(j+100*i < frequencyList.currentModel.entries().length) testModel.append(frequencyList.currentModel.entries()[j+100*i])
                        }
                    }
                }
            }
        }
    }/**/
    
    
}
