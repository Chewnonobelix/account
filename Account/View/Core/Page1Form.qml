import QtQuick 2.15
//import QtQuick.Controls 1.4
import QtQuick.Layouts 1.13
//import QtQuick.Controls.Styles 1.4

import QtQuick.Controls 2.15
import QtQuick.Window 2.13

//import Style 1.0
import "../Style"
import "../Functionnal"
import "../Budget"

import Account 1.0

Page {

    
    implicitWidth: parent.width
    implicitHeight: parent.height
    
    id: pageTable
    
    property int v_dateMonth: cal.currentMonth + 1
    property int v_dateYear: cal.currentYear

    background: AccountBackground {
        invisible: true
    }
    
    MouseArea {
        acceptedButtons: Qt.NoButton
        hoverEnabled: true

        onMouseXChanged: addingid.pX = (mouseX - pageTable.x) / pageTable.width
        onMouseYChanged: addingid.pY = (mouseY - pageTable.y) / pageTable.height
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

            onUpdateSelected:  {
                _main = selectedDates
                //                view.unselectAll()
            }

            onDatesChanged:  {
                view.reset()
                _main.updateQuickView()
                _main.pageChange()

                if(_budget)
                    _budget.calDateChange(selectedDates)
            }

            onMonthChanged: {
                _main.previewCalendar(currentMonth, currentYear)
            }

            Connections {
                target: _main

                function onClearCalendar() {
                    cal.clear()
                }

                function onAppendMonthPreview(val) {
                    cal.addMonth(val)
                }

                function onAppendCalendarPreview(val) {
                    cal.addCalendar(val)
                }
            }
        }
        
        AccountButton {
            id: add
            text: qsTr("Add")


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

            //            property int index: view.currentRow
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

            HorizontalHeaderView {
                id: headerView
//                height: view.height*0.15
//                rowHeightProvider: height
                syncView: view
                z:5
                clip: true
                delegate: AccountHeader {
                    text: display
                    clip: true
//                    height: headerView.height
                }
            }

            TableView {

                height: parent.height * 0.85
                width: parent.width
                id: view
                model: _mainModel
                clip: true

                property var columns: [width*0.10,
                    width*0.23,
                    width*0.23,
                    width*0.22,
                    width*0.22]
                onHeightChanged: forceLayout()
                rowHeightProvider: -1
                columnWidthProvider: function(column) {
                    return columns[column]
                }

                rowSpacing: height* 0.02
                delegate: AccountLabel {
                    required property var type

                    clip: true
                    Component.onCompleted: {
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log(width, row, column, text)
                            _mainModel.currentIndex = _mainModel.currentIndex === row ? -1 : row
                        }
                    }

                    height: view.height*0.05
//                    width: view.columnWidthProvider(column)
                    //                    width: view.width *0.15
                    text: _mainModel.at(row,column)


                    background: Rectangle {
                        anchors.fill: parent
                        gradient: row === _mainModel.currentIndex ? type === Account.Income ? AccountStyle.selectViewIn : AccountStyle.selectViewOut : AccountStyle.unselectView
                    }
                }
            }
            
            PageChanger {
                id: changer
                objectName: "pageSkip"
                height: parent.height * 0.05
                width: parent.width

                Connections {
                    target: _main

                    function onMaxPageChanged(max) {
                        changer.maxPage = max
                    }
                }

                onPageChange:  {
                    view.unselectAll()
                    _main.currentPage = pageIndex
                    _main.pageChange()
                }
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
        //        view.setNewIndex(-1)
        _main.exec()
        //        currentId = Qt.binding(function() {return view.currentEntry && !view.currentEntry.isBlocked ? view.currentEntry.id : -1})
    }
    
    //    property var currentId: view.selection.count !== 0 && !model[view.currentRow].isBlocked ? model[view.currentRow].id : null
}
