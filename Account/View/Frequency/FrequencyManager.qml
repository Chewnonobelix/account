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
    
    ListModel {
        id: testModel
    }
    
    title: qsTr("Frequency manager")
    
    maximumHeight: screen.height / 2
    maximumWidth: screen.width * 0.4
    visible: false
    
    width: maximumWidth
    height: maximumHeight
    
    id: root
    
    Component.onCompleted: {
        layout.usableHeight = Qt.binding(function() {return height})
        layout.usableWidth = Qt.binding(function() {return width})      
    }
    
    Rectangle {
        anchors.fill: parent
        gradient: pageStyle.backgroundGradient
        
        
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
            
            Control2.Label {
                text: qsTr("Reference") + " ->"
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                
                Layout.preferredHeight: parent.usableHeight * 0.05
                Layout.preferredWidth: parent.usableWidth * 0.20
                Layout.row: 0
                Layout.column: 0
                Layout.columnSpan: 2
                Layout.rowSpan: 1
                
                background: Rectangle {
                    gradient: pageStyle.goldHeader
                }
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
                
                header:  Control2.Label {
                    text: qsTr("Frequency list")
                    font.family: pageStyle.title.name
                    font.pixelSize: pageStyle.title.size2
                    fontSizeMode: Text.Fit
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    
                    background: Rectangle {
                        gradient: pageStyle.goldHeader
                    }
                }
                                
                Component.onCompleted: {
                    currentIndex = Qt.binding(function(){ return count === 0 ? -1 : currentIndex })                   
                    ref.entry = Qt.binding(function() {if(currentIndex !== -1)return model[currentIndex].reference})
                    
                    
                    whenCombo.currentIndex = Qt.binding(function() {return whenCombo.model.findIndex(model[currentIndex].freq + 0)})         
                    groupText.nb = Qt.binding(function() {return model[currentIndex].nbGroup})
                    countText.nb = Qt.binding(function() {return model[currentIndex].listEntries().length})
                    pageChanger.maxPage = Qt.binding(function() {return countText.nb / 100 + 1})
                    whenCombo.enabled = Qt.binding(function() {return count !== 0})                                    
                }
                
                clip: true
                Rectangle {
                    anchors.fill: parent
                    border.color: "gold"
                    color: "transparent"
                }
                
                
                onCurrentIndexChanged: {
                    if(enabled) {                        
                        pageChanger.pageIndex = 1
                        
                        dateText.from = model[currentIndex].listEntries()[0] ? model[currentIndex].listEntries()[0].date : new Date()
                        dateText.to = dateText.from
                        
                        for(var i = 0; i < model[currentIndex].listEntries().length; i++) {                            
                            if(model[currentIndex].listEntries()[i].date < dateText.from)
                                dateText.from = model[currentIndex].listEntries()[i].date
                            
                            if(model[currentIndex].listEntries()[i].date > dateText.to)
                                dateText.to = model[currentIndex].listEntries()[i].date
                        }
                        
                        pageChanger.s_pageChange()
                    }
                }
                
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
                
                Layout.preferredHeight: parent.usableHeight * 0.05
                Layout.preferredWidth: parent.usableWidth * 0.09
                Layout.row: 4
                Layout.column: 0
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                
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
                
                Layout.preferredHeight: parent.usableHeight * 0.05
                Layout.preferredWidth: parent.usableWidth * 0.09
                Layout.row: 4
                Layout.column: 1
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                
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
                
                onS_valueChanged: if(entry && enabled) valueChanged(entry.id, value)
                onS_titleChanged: if(entry && enabled) titleChanged(entry.id, title)
                onS_catChanged: if(entry && enabled) catChanged(entry.id, cat, "manager")
                
                onEntryChanged: {
                    linked.reloadCat()
                    reloadCat()
                }
                
                Component.onCompleted: {
                    enabled = Qt.binding(function() {return frequencyList.count !== 0})
                    entry = Qt.binding(function() {if(frequencyList.currentIndex !== -1)return frequencyList.model[frequencyList.currentIndex].reference})
                    
                    catModel = Qt.binding(function(){return (entry && entry.type === "income") ? incomeList : outcomeList})
                    typeCombo.currentIndex = Qt.binding(function(){return (entry && entry.type === "income") ? 0 : 1})
                    linked.catModel = Qt.binding(function() { return catModel})             
                }
            }
            
            Control2.ComboBox { 
                id: whenCombo
                objectName: "whenCombo"
                
                Layout.preferredHeight: parent.usableHeight * 0.05
                Layout.preferredWidth: parent.usableWidth * 0.23
                Layout.row: 1
                Layout.column: 2
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                
                enabled: ref.enabled
                
                
                model: models.freqModel
                textRole: "name"
                signal s_freq(int i, int f)
                
                onCurrentIndexChanged: {
                    if(down && ref.entry) s_freq(ref.entry.id, model.get(currentIndex).role)
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
                
                Layout.preferredHeight: parent.usableHeight * 0.05
                Layout.preferredWidth: parent.usableWidth * 0.24
                Layout.row: 1
                Layout.column: 3
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                
                enabled: ref.enabled
                
                
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
                
                Layout.preferredHeight: parent.usableHeight * 0.05
                Layout.preferredWidth: parent.usableWidth * 0.23
                Layout.row: 1
                Layout.column: 4
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                
                enabled: ref.enabled
                
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
                
                Text {
                    id: groupText
                    property int nb: 0
                    text: qsTr(" Number of generation") + ": " + nb 
                    fontSizeMode: Text.Fit
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                    width: parent.width
                    height: parent.height / parent.children.length
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
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
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    height: parent.height / parent.children.length                    
                }
                Text {
                    
                    id: dateText
                    property var from
                    property var to
                    anchors.top: countText.bottom
                    text: qsTr("From") + " " + Qt.formatDate(from, "dd-MM-yyyy") + ", " + qsTr("to") + " " + Qt.formatDate(to, "dd-MM-yyyy")
                    fontSizeMode: Text.Fit
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                    width: parent.width
                    height: parent.height / parent.children.length
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
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
                
                highlight: Rectangle {
                    gradient: pageStyle.calSelect
                }
                
                highlightMoveDuration: 0
                delegate: Control2.ItemDelegate {
                    objectName: "entryDel"
                    background: Rectangle {
                        anchors.fill: parent
                        property var index: parent.index
                        gradient: pageStyle.unselectView  
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
                    
                    Label {                        
                        anchors.fill: parent
                        text: Qt.formatDate(date, "dd-MM-yyyy")
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        fontSizeMode: Text.Fit
                        font.family: pageStyle.core.name
                        font.pixelSize: pageStyle.core.size
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
                    
                    for(var j = 0 ; j < 100; j++) {
                        if(j+100*i < frequencyList.model[frequencyList.currentIndex].listEntries().length) testModel.append(frequencyList.model[frequencyList.currentIndex].listEntries()[j+100*i])
                    }
                }
            }
        }
    }/**/
    
    
}
