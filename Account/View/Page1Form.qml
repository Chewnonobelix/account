import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4

Page {

    implicitWidth: parent.width
    implicitHeight: parent.height

    id: pageTable

    property date v_date

    AccountStyle {
        id: pageStyle
    }

    background: Rectangle {
        color: "transparent"
    }

    MultiCalendar {
        id: cal
        objectName: "cal"
        weekNumbersVisible: true
        implicitHeight: parent.height /2
        implicitWidth: parent.width * 0.2
    }

    /* */

    Adding {
        id: addingid
        objectName: "addingid"
        y: pY > -1 ? (mainWindow.height * pY) - Math.max(0, mY) : (parent.height / 2 + add.height / 2 + 10)
        x: pY > -1 ? (mainWindow.width * pX) - Math.max(0, mX): (add.width / 2)
        property double pX: -1
        property double pY: -1
        property double mX: -1
        property double mY: -1
        Component.onCompleted:   {
            reset()
        }

        function setSize(px, py) {
            mX = ((mainWindow.width * px)+width)-mainWindow.width
            mY = ((mainWindow.height * py)+height)-(mainWindow.height)

            pX = px
            pY= py
        }
    }


    Component.onCompleted: {
        addingid.close()
    }

    function openAdding(pX, pY) {

        addingid.setSize(pX, pY)

        if(cal.selectedDates.length > 0) {
            for(var index in cal.selectedDates){
                addingid.addDate(cal.selectedDates[index])
            }
        }
        else {
            addingid.addDate(Qt.formatDate(new Date(), "dd-MM-yyyy"))
        }
        addingid.open()
    }

    Item {
        anchors.top: cal.bottom
        anchors.topMargin: 10
        width: cal.width
        height: edit.height + add.height + 10
        id: group
        enabled: accountSelect.model.length > 0


        Button {
            id: add
            text: qsTr("Add")
            width: parent.width * .45
            anchors.top: parent.top
            anchors.left: parent.left
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size

            Shortcut {
                context: Qt.ApplicationShortcut
                onActivated:  {
                    add.released()
                }
                sequence: "A"
            }



            Rectangle {
                id: rectAdd
                anchors.fill: parent
                gradient: pageStyle.goldButton
            }

            onPressed: {
                rectAdd.gradient = pageStyle.darkGoldButton
            }

            onReleased: {
                rectAdd.gradient = pageStyle.goldButton
                mainWindow.adding(false)
            }

        }


        Button {
            id: remove
            text: qsTr("Remove")
            width: parent.width * .45
            anchors.left: add.right
            anchors.leftMargin: (parent.width * .1)
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size
            property int index: view.currentIndex

            Shortcut {
                context: Qt.ApplicationShortcut
                onActivated:  {
                    remove.released()
                }
                sequence: "R"
            }

            Rectangle {
                id: rectRemove

                anchors.fill: parent
                gradient: pageStyle.goldButton
            }

            onPressed: {
                rectRemove.gradient = pageStyle.darkGoldButton
            }

            onReleased: {
                rectRemove.gradient = pageStyle.goldButton
                if(index > -1) {
                    mainWindow.remove(defaultModel.get(index).id)
                }
            }
        }
    }



    ListModel {
        id: defaultModel
        objectName: "defaultModel"

    }


    TableView {
        anchors.left: cal.right
        anchors.leftMargin: 5
        height: parent.height
        width: parent.width * 0.25
        id: view
        objectName: "entryView"
        model: defaultModel

        sortIndicatorVisible:  true
        property string currentType
        property int currentIndex: -1

        function fAdd(i) {
            defaultModel.append(i)
        }


        function unselectAll() {
            if(rowCount >= 0) {
                selection.deselect(0, rowCount-1)
            }
            currentIndex = -1
        }

        function reset() {
            defaultModel.clear()
            currentIndex = -1
            infoView.enabled = false
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
            positionViewAtRow(0, ListView.Beginning )
        }

        TableViewColumn {
            role: "id"
            visible: false
        }

        TableViewColumn {
            role: "estimated"
            visible: false
        }

        TableViewColumn {
            role: "type"
            title: qsTr("[+/-]")
            width: 40
            movable: false
            resizable: false
            id: typeColumn
            property string tipText: "*: estimated entry"
            delegate: Rectangle {
                color: "transparent"
                anchors.centerIn: parent
                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    propagateComposedEvents: true
                    onClicked: {
                        var i = styleData.row !== view.currentIndex ? styleData.row : -1

                        view.selection.clear()
                        view.currentIndex = i
                        if(i >= 0) {
                            view.selection.select(i)
                        }
                    }
                }
                Label {
                    property string est: defaultModel.get(styleData.row).estimated ? "*" : ""
                    text: styleData.value === "income" ? "+"+est:"-"+est
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
            width: 100
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
                        var i = styleData.row !== view.currentIndex ? styleData.row : -1
                        view.selection.clear()
                        view.currentIndex = i
                        if(i >= 0) {
                            view.selection.select(i)
                        }
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
            width: 100
            movable: false
            resizable: false
        }

        TableViewColumn {
            role: "label"
            title: qsTr("Label")
            width: 100
            movable: false
            resizable: false
        }


        TableViewColumn {
            role: "total"
            title: qsTr("Total")
            width: 100
            movable: false
            resizable: false
        }


        headerDelegate: Rectangle {
            gradient:  pageStyle.goldHeader

            height: view.height * 0.03
            anchors.centerIn: parent
            anchors.leftMargin: 10
            property string isHovered: styleData.containsMouse


            Label {
                id: headerText
                height: parent.height*.8
                anchors.centerIn: parent
                text: styleData.value
                font.family: pageStyle.title.name
                font.pixelSize: height * 0.85

                ToolTip.visible: isHovered && (styleData.column === 2)
                ToolTip.text: view.getToolTip(styleData.column)
                ToolTip.delay: 500
            }
        }

        function getToolTip(index) {
            if(index === 2) {
                return typeColumn.tipText
            }

            return ""
        }

        itemDelegate: Rectangle {
            width: parent.width
            height: 20
            anchors.fill: parent
            color: "transparent"
            Label {
                id: textItem
                text: styleData.value


                anchors.centerIn: parent

                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size

            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor

                onClicked: {
                    if(view.selection.contains(styleData.row)) {
                        view.selection.clear()
                    }
                    else {
                        view.selection.clear()
                        view.selection.select(styleData.row)
                    }

                    if(view.currentIndex !== styleData.row) {
                        view.currentIndex = styleData.row
                        view.s_view(defaultModel.get(view.currentIndex).id)
                    }
                    else {
                        view.unselectAll()
                    }


                    infoView.enabled = (view.currentIndex !== -1) && (defaultModel.get(view.currentIndex).label !== "Initial")
                    remove.enabled = (view.currentIndex !== -1) && (defaultModel.get(view.currentIndex).label !== "Initial")
                }
            }
        }

        rowDelegate: Rectangle {
            id:rectRow

            width: parent.width
            height: 20


            gradient: styleData.selected ? defaultModel.get(styleData.row).type === "outcome" ? pageStyle.selectViewOut : pageStyle.selectViewIn : pageStyle.unselectView
        }
    }

    ScrollView {

        contentWidth: infoView.width
        contentHeight: infoView.height
        anchors.left: view.right
        width: parent.width*0.55
        height: parent.height


        anchors.leftMargin: 10


        clip: true
        InformationView {
            id: infoView
            objectName: "infoView"
            width: 600
            height: pageTable.height
            clip: true
            enabled: false
        }



    }

}

