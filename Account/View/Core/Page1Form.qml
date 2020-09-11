import QtQuick 2.13
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.13
import QtQuick.Controls.Styles 1.4

import QtQuick.Controls 2.13
import QtQuick.Window 2.13

//import Style 1.0
import "../Style"
import "../Functionnal"
import "../Budget"

Page {

    
    implicitWidth: parent.width
    implicitHeight: parent.height
    
    id: pageTable
    
    property int v_dateMonth: cal.currentMonth + 1
    property int v_dateYear: cal.currentYear
        
    background: AccountBackground {
        invisible: true
   }
    
    GridLayout {
        id: grid
        anchors.fill: parent
        rowSpacing: pageTable.height * 0.02
        columnSpacing: pageTable.width * 0.02
        anchors.topMargin: pageTable.height * 0.02
        anchors.bottomMargin: pageTable.height * 0.02
        anchors.rightMargin: pageTable.width * 0.02
        anchors.leftMargin: pageTable.width * 0.02
        
        
        ZoomCalendar {
            id: cal
            objectName: "cal"
            
            Layout.column: 0
            Layout.row: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 2
            Layout.preferredHeight: pageTable.height * 0.30
            Layout.preferredWidth: pageTable.width * 0.20

            onUpdateSelected: view.unselectAll()
            onDatesChanged:  {
                view.reset()
                _main.updateQuickView()
                _main.pageChange()

                if(_budget)
                    _budget.calDateChange(selectedDates)
            }

            onMonthChanged: {
                _main.previewCalendar()
            }
        }
        
        AccountButton {
            id: add
            text: qsTr("Add1")


            enabled: accountSelect.model.length > 0
            Layout.column: 0
            Layout.row: 1
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: pageTable.height * 0.05
            Layout.preferredWidth: pageTable.width * 0.09
            
            ToolTip.text: qsTr("Add new transaction")
                        
            onClicked: {
                _main.add(false)
            }
        }
        
        AccountButton {
            id: remove
            text: qsTr("Remove")

            property int index: view.currentRow
            enabled: infoView.visible
            Layout.column: 1
            Layout.row: 1
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: pageTable.height * 0.05
            Layout.preferredWidth: pageTable.width * 0.09
            
            ToolTip.text: qsTr("Remove select transaction")
            
            onClicked: {
                if (enabled) {
                    _main.remove(pageTable.currentId)
                }
            }
        }
        
        Column {
            Layout.column: 0
            Layout.row: 2
            Layout.rowSpan: 1
            Layout.columnSpan: 2
            Layout.preferredHeight: pageTable.height * 0.57
            Layout.preferredWidth: pageTable.width * 0.20
            spacing: pageTable.height * .02
            
            AccountHeader {
                id: quickViewDate
                objectName: "quickViewDate"
                property date currentDate: new Date()
                
                text: qsTr("Budget quick view") + ": " + Qt.formatDate(currentDate, "dd-MM-yyyy")
                height: parent.height * .10
                width: parent.width
                clip: true
            }
            
            Loader {
                id: loadQuick
                Connections {
                    target: _main

                    function onEnableQuickView(enable) {
                        loadQuick.active = true
                    }
                }

                active: false
                width: parent.width
                height: parent.height - parent.spacing - quickViewDate.height

                onActiveChanged: {
                    item.width = width
                    item.height = height
                }
                source: "../Budget/BudgetView.qml"
            }
        }
        
        Column {
            id: colTable
            Layout.column: 2
            Layout.row: 0
            Layout.rowSpan: 3
            Layout.columnSpan: 1
            
            Layout.preferredHeight: pageTable.height * 0.96
            Layout.preferredWidth: pageTable.width * 0.20
            spacing: 0
            TableView {
                height: parent.height * 0.95
                width: parent.width
                id: view
                objectName: "entryView"
                model: []
                
                ToolTip.text: qsTr("Transactions list")
                ToolTip.visible: tableArea.containsMouse && rowAt(tableArea.mouseX, tableArea.mouseY) === -1
                ToolTip.delay: 500
                ToolTip.timeout: 1000
                
                MouseArea {
                    id: tableArea
                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.NoButton
                }

                Component.onCompleted: {
                    selection.clear()
                    currentEntry = Qt.binding( function(){ return selection.count !== 0 ? model[currentRow] : null } )
                }
                
                onModelChanged: selection.clear()
                
                horizontalScrollBarPolicy: Qt.ScrollBarAsNeeded
                
                sortIndicatorVisible: true
                
                function unselectAll() {
                    selection.clear()
                }
                
                function reset() {
                    infoView.visible = false
                }
                
                signal s_view(var index)
                
                property var currentEntry
                
                backgroundVisible: false
                
                onWidthChanged: {
                    flickableItem.contentX = 0
                }
                
                onHeightChanged: {
                    flickableItem.contentY = 0
                }
                
                TableViewColumn {
                    role: "id"
                    visible: false
                    width: 0
                }
                
                TableViewColumn {
                    role: "estimated"
                    visible: false
                    width: 0
                }
                
                function setNewIndex(index) {
                    if (selection.contains(index) || index === -1) {
                        selection.clear()
                        _main.edit(null)
                    } else {
                        selection.clear()
                        currentRow = index
                        selection.select(index)
                        _main.edit(currentEntry.id)
                    }
                }
                
                function selectFromId(id) {
                    for (var i = 0; i < model.length; i++) {
                        if (model[i].id === id) {
                            setNewIndex(i)
                        }
                    }
                }
                
                TableViewColumn {
                    role: "type"
                    title: "[+/-]"
                    width: 45
                    movable: false
                    resizable: false
                    id: typeColumn
                    property string tipText: "*:" + qsTr("estimated entry")
                    delegate: Rectangle {
                        color: "transparent"
                        anchors.centerIn: parent
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            propagateComposedEvents: true
                            
                            onClicked: {
                                view.setNewIndex(styleData.row)
                            }
                        }
                        AccountLabel {
                            property string est: view.model[styleData.row] && view.model[styleData.row].info.estimated ? "*" : ""
                            
                            text: styleData.value === "income" ? "+" + est : "-" + est
                            anchors.fill: parent
                            
                        }
                    }
                }
                
                TableViewColumn {
                    role: "date"
                    title: qsTr("Date")
                    width: (Screen.width * .20 - 45) / 4
                    movable: false
                    resizable: false
                    id: columnDate
                    
                    delegate: Rectangle {
                        color: "transparent"
                        anchors.centerIn: parent
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            propagateComposedEvents: true
                            onClicked: {
                                view.setNewIndex(styleData.row)
                            }
                        }
                        
                        AccountLabel {
                            text: Qt.formatDate(styleData.value, "dd-MM-yyyy")
                            anchors.fill: parent
                        }
                    }
                }
                
                TableViewColumn {
                    role: "value"
                    title: qsTr("Value")
                    width: (Screen.width * .20 - 45) / 4
                    movable: false
                    resizable: false
                    
                    delegate: Rectangle {
                        color: "transparent"
                        anchors.centerIn: parent
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            propagateComposedEvents: true
                            onClicked: {
                                view.setNewIndex(styleData.row)
                            }
                        }
                        
                        AccountLabel {
                            text: styleData.value
                            clip: true
                            anchors.fill: parent
                        }
                    }
                }
                
                TableViewColumn {
                    role: "label"
                    title: qsTr("Label")
                    width: (Screen.width * .20 - 45) / 4
                    movable: false
                    resizable: false
                    id: labelHeader
                    
                    delegate: Rectangle {
                        color: "transparent"
                        anchors.centerIn: parent
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            propagateComposedEvents: true
                            onClicked: {
                                view.setNewIndex(styleData.row)
                            }
                        }
                        
                        AccountLabel {
                            text: styleData.value
                            clip: true
                            anchors.fill: parent
                        }
                    }
                }
                
                TableViewColumn {
                    role: "total"
                    title: qsTr("Total")
                    width: (Screen.width * .20 - 45) / 4
                    movable: false
                    resizable: false
                    
                    delegate: Rectangle {
                        color: "transparent"
                        anchors.centerIn: parent
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            propagateComposedEvents: true
                            onClicked: {
                                view.setNewIndex(styleData.row)
                            }
                            hoverEnabled: true
                        }
                        
                        AccountLabel {
                            text: styleData.value.value
                            clip: true
                            anchors.fill: parent
                        }
                    }
                }
                
                headerDelegate: Rectangle {
                    gradient: styleData.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldHeader
                    
                    height: view.height * 0.03
                    anchors.centerIn: parent
                    anchors.leftMargin: 10
                    
                    border.color: "darkgoldenrod"
                    AccountLabel {
                        id: headerText
                        height: parent.height * .8
                        anchors.centerIn: parent
                        text: styleData.value
                        font.family: AccountStyle.title.name
                        font.pixelSize: height * 0.85
                        
                        ToolTip.visible: styleData.containsMouse && (styleData.column === 2)
                        ToolTip.text: view.getToolTip(styleData.column)
                        ToolTip.delay: 500
                    }
                }
                
                signal s_sortRole(string role)
                signal s_sortOrder(int order)

                onSortIndicatorColumnChanged: {
                   _main.sortRole(getColumn(sortIndicatorColumn).role)
                }
                
                onSortIndicatorOrderChanged: {
                    _main.sortOrder(sortIndicatorOrder)
                }
                
                function getToolTip(index) {
                    if (index === 2) {
                        return typeColumn.tipText
                    }
                    
                    return ""
                }
                
                itemDelegate: Rectangle {
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            view.setNewIndex(styleData.row)
                        }
                    }
                }
                
                rowDelegate: Rectangle {
                    id: rectRow
                    
                    width: view.width
                    height: view.height * .03
                    
                    gradient: view.model[styleData.row] && styleData.selected ? view.model[styleData.row].type === "outcome" ? AccountStyle.selectViewOut : AccountStyle.selectViewIn : AccountStyle.unselectView
                    
                }
                
                onCurrentRowChanged: {
                    setNewIndex(currentRow)
                }
                
                function swap(i,j) {
                    var t = model[i]
                    model[i] = model[j]
                    model[j] = t
                }
                
                function sort(role, roleorder) {
                    for (var i = 0; i < rowCount; i++) {
                        for (var j = i; j < rowCount; j++) {
                            if (roleorder === Qt.AscendingOrder) {
                                if (model[j][role] < model[i][role]) {
                                    swap(i, j)
                                }
                            } else {
                                if (model[j][role] > model[i][role]) {
                                    swap(i, j)
                                }
                            }
                        }
                    }
                    model = model
                }
            }
            
            PageChanger {
                id: changer
                objectName: "pageSkip"
                height: parent.height * 0.05
                width: parent.width

                onPageChange: _main.pageChange()
            }
        }
        
        ScrollView {
            Layout.column: 3
            Layout.row: 0
            Layout.rowSpan: 3
            Layout.columnSpan: 1
            
            Layout.preferredWidth: pageTable.width * 0.52
            Layout.preferredHeight: pageTable.height * .96
            ScrollBar.horizontal.policy: ScrollBar.AsNeeded
            
            clip: true
            InformationView {
                id: infoView
                objectName: "infoView"
                clip: true
                enabled: true
                
                implicitWidth: Screen.width * 0.52
                implicitHeight: grid.height
            }
        }
        
    }
    
    
    /* */
    Adding {
        id: addingid
        objectName: "addingid"
        y: pY > 0 ? (mainWindow.height * pY) - Math.max(0, mY) : (parent.height / 2 + add.height / 2 + 10)
        x: pY > 0 ? (mainWindow.width * pX) - Math.max(0, mX) : (add.width / 2)
        property double pX: -1
        property double pY: -1
        property double mX: ((mainWindow.width * pX) + width) - mainWindow.width
        property double mY: ((mainWindow.height * pY) + height) - (mainWindow.height)
        
        implicitWidth: parent.width * .35
        implicitHeight: parent.height * .12

        Component.onCompleted: {
            reset()
        }

        onOpened: {
            openDate = cal.selectedDates.length === 0 ? new Date() : Date.fromLocaleString(Qt.locale(), cal.selectedDates[0], "dd-MM-yyyy")
        }
    }
    
    
    Component.onCompleted: {
        _main.exec()
        currentId = Qt.binding(function() {return view.currentEntry && !view.currentEntry.isBlocked ? view.currentEntry.id : -1})
    }
    
    property var currentId: view.selection.count !== 0 && !model[view.currentRow].isBlocked ? model[view.currentRow].id : null
}
