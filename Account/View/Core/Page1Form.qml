import QtQuick 2.13
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.13
import QtQuick.Controls.Styles 1.4

import QtQuick.Controls 2.4
import QtQuick.Window 2.13

import "../Budget" as B
import "../Style" as S
import "../Frequency" as F

Page {
    
    implicitWidth: parent.width
    implicitHeight: parent.height
    
    id: pageTable
    
    property int v_dateMonth: cal.currentMonth + 1
    property int v_dateYear: cal.currentYear
    
    S.AccountStyle {
        id: pageStyle
    }
    
    background: Rectangle {
        color: "transparent"
    }
    
    GridLayout {
        id: grid
        anchors.fill: parent
        rowSpacing: ageTable.height * 0.02
        columnSpacing: pageTable.width * 0.02
        anchors.topMargin: pageTable.height * 0.02
        anchors.bottomMargin: pageTable.height * 0.02
        anchors.rightMargin: pageTable.width * 0.02
        anchors.leftMargin: pageTable.width * 0.02
        
        
        MultiCalendar {
            id: cal
            objectName: "cal"
            weekNumbersVisible: true

            Layout.column: 0
            Layout.row: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 2
            Layout.preferredHeight: pageTable.height * 0.30
            Layout.preferredWidth: pageTable.width * 0.20
        }

        Button {
            id: add
            text: qsTr("Add")
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size
            enabled: accountSelect.model.length > 0
            Layout.column: 0
            Layout.row: 1
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: pageTable.height * 0.05
            Layout.preferredWidth: pageTable.width * 0.09

            ToolTip.text: qsTr("Add new transaction")
            ToolTip.visible: hovered
            ToolTip.delay: 500

            Rectangle {
                id: rectAdd
                anchors.fill: parent
                gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
            }

            MouseArea {
                z: -1
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                acceptedButtons: Qt.NoButton
            }

            onClicked: {
                mainWindow.adding(false)
            }
        }

        Button {
            id: remove
            text: qsTr("Remove")
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size
            property int index: view.currentIndex
            enabled: view.currentIndex !== -1 && accountSelect.model.length > 0

            Layout.column: 1
            Layout.row: 1
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: pageTable.height * 0.05
            Layout.preferredWidth: pageTable.width * 0.09

            ToolTip.text: qsTr("Remove select transaction")
            ToolTip.visible: hovered
            ToolTip.delay: 500

            MouseArea {
                z: -1
                anchors.fill: parent
                cursorShape: parent.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
                acceptedButtons: Qt.NoButton
            }

            Rectangle {
                id: rectRemove

                anchors.fill: parent
                gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
            }
            onClicked: {
                if (index > -1) {
                    mainWindow.remove(pageTable.currentId)
                }
            }
        }

        Column {
            Layout.column: 0
            Layout.row: 2
            Layout.rowSpan: 1
            Layout.columnSpan: 2
            Layout.preferredHeight: pageTable.height * 0.61
            Layout.preferredWidth: pageTable.width * 0.20
            spacing: pageTable.height * .02

            Label {
                id: quickViewDate
                objectName: "quickViewDate"
                property date currentDate
                background: Rectangle {
                    gradient: pageStyle.goldHeader
                }

                text: qsTr("Budget quick view") + ": " + Qt.formatDate(currentDate, "dd-MM-yyyy")
                height: parent.height * .10
                width: parent.width
                fontSizeMode: Text.Fit
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                clip: true
            }

            B.BudgetView {
                id: budgetQuick
                objectName: "budgetQuick"

                width: parent.width
                height: parent.height - parent.spacing - quickViewDate.height
            }
        }

        Column {
            id: colTable
            Layout.column: 2
            Layout.row: 0
            Layout.rowSpan: 3
            Layout.columnSpan: 1
//            Layout.preferredHeight: grid.height
            Layout.fillHeight: true
            Layout.preferredWidth: pageTable.width * 0.20
            spacing: 0
            TableView {
                height: parent.height * 0.95
                width: parent.width
                id: view
                objectName: "entryView"
                model: []

                property int maximumWidth: 4 * 100 + 60

                horizontalScrollBarPolicy: Qt.ScrollBarAsNeeded

                sortIndicatorVisible: true
                property string currentType
                property int currentIndex: -1

                function unselectAll() {
                    selection.clear()
                    currentIndex = -1
                }

                function reset() {
                    currentIndex = -1
                    infoView.visible = false
                }

                signal s_view(int index)

                backgroundVisible: false
                Connections {
                    target: cal
                    onS_datesChanged: {
                        view.reset()
                    }
                }

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
                    if (selection.contains(index)) {
                        selection.clear()
                        currentIndex = -1
                    } else {
                        selection.clear()
                        currentIndex = index
                        selection.select(index)
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
                    title: qsTr("[+/-]")
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
                        Label {
                            property string est: view.model[styleData.row].info.estimated ? "*" : ""

                            text: styleData.value === "income" ? "+" + est : "-" + est
                            font.family: pageStyle.core.name
                            font.pixelSize: pageStyle.core.size
                            horizontalAlignment: Text.AlignHCenter
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

                        Label {
                            text: Qt.formatDate(styleData.value, "dd-MM-yyyy")
                            font.family: pageStyle.core.name
                            font.pixelSize: pageStyle.core.size
                            horizontalAlignment: Text.AlignHCenter
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

                        Label {
                            text: styleData.value
                            clip: true
                            font.family: pageStyle.core.name
                            font.pixelSize: pageStyle.core.size
                            horizontalAlignment: Text.AlignHCenter
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

                        Label {
                            text: styleData.value
                            clip: true
                            font.family: pageStyle.core.name
                            font.pixelSize: pageStyle.core.size
                            verticalAlignment: Text.AlignVCenter
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

                        Label {
                            text: styleData.value
                            clip: true
                            font.family: pageStyle.core.name
                            font.pixelSize: pageStyle.core.size
                            horizontalAlignment: Text.AlignHCenter
                            anchors.fill: parent
                        }
                    }
                }

                headerDelegate: Rectangle {
                    gradient: isClicked ? pageStyle.darkGoldButton : pageStyle.goldHeader

                    height: view.height * 0.03
                    anchors.centerIn: parent
                    anchors.leftMargin: 10
                    property bool isHovered: styleData.containsMouse
                    property bool isClicked: styleData.pressed

                    border.color: "darkgoldenrod"
                    Label {
                        id: headerText
                        height: parent.height * .8
                        anchors.centerIn: parent
                        text: styleData.value
                        font.family: pageStyle.title.name
                        font.pixelSize: height * 0.85

                        ToolTip.visible: isHovered && (styleData.column === 2)
                        ToolTip.text: view.getToolTip(styleData.column)
                        ToolTip.delay: 500
                    }
                }

                onSortIndicatorColumnChanged: {
                    sort(getColumn(sortIndicatorColumn).role,
                                      sortIndicatorOrder)
                    if (currentIndex !== -1) {
                        s_view(model[currentIndex].id)
                    }
                }

                onSortIndicatorOrderChanged: {
                    sort(getColumn(sortIndicatorColumn).role,
                                      sortIndicatorOrder)

                    if (currentIndex !== -1) {
                        s_view(model[currentIndex].id)
                    }
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
                    height: 20

                    gradient: styleData.selected ? view.model[styleData.row].type === "outcome" ? pageStyle.selectViewOut : pageStyle.selectViewIn : pageStyle.unselectView
                    
                }

                onCurrentIndexChanged: {
                    var cItem =  model[currentIndex]
                    infoView.visible = (currentIndex !== -1) && (cItem.label !== "Initial")
                            
                    remove.enabled = infoView.visible

                    if (currentIndex !== -1)
                        s_view(cItem.id)
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
            }

        }

        ScrollView {

            
            Layout.column: 3
            Layout.row: 0
            Layout.rowSpan: 3
            Layout.columnSpan: 1
//            Layout.preferredHeight: grid.height
            
            Layout.preferredWidth: pageTable.width * 0.52
            

            ScrollBar.horizontal.policy: ScrollBar.AsNeeded

            clip: true
            InformationView {
                id: infoView
                objectName: "infoView"
                clip: true
                visible: false
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
        y: pY > 0 ? (mainWindow.height * pY) - Math.max(
                        0, mY) : (parent.height / 2 + add.height / 2 + 10)
        x: pY > 0 ? (mainWindow.width * pX) - Math.max(0, mX) : (add.width / 2)
        property double pX: -1
        property double pY: -1
        property double mX: -1
        property double mY: -1
        Component.onCompleted: {
            reset()
        }
        
        function setSize(px,py) {
            mX = ((mainWindow.width * px) + width) - mainWindow.width
            mY = ((mainWindow.height * py) + height) - (mainWindow.height)
            
            pX = px
            pY = py
        }
    }
    
    Component.onCompleted: {
        addingid.close()
    }
    
    function openAdding(pX, pY) {
        
        addingid.setSize(pX, pY)
        
        if (cal.selectedDates.length > 0) {
            for (var index in cal.selectedDates) {
                addingid.addDate(cal.selectedDates[index])
            }
        } else {
            addingid.addDate(Qt.formatDate(new Date(), "dd-MM-yyyy"))
        }
        addingid.open()
    }
    
    property int currentId: view.currentIndex > -1 && model[view.currentIndex].label !== "Initial" ? model[view.currentIndex].id : -1
        
}
