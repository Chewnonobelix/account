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

        Component.onCompleted:   {
            reset()
        }
    }

    function openAdding() {
        if(cal.selectedDates.length > 0) {
            for(var index in cal.selectedDates){
                addingid.addDate(cal.selectedDates[index])
            }
        }
        else {
            addingid.addDate(Qt.formatDate(new Date(), "dd-MM-yyyy"))
        }
        addingid.open()
        return 0
    }

    Item {
        anchors.top: cal.bottom
        anchors.topMargin: 10
        width: cal.width

        Button {
            id: add
            text: qsTr("Add")
            width: parent.width * .45
            anchors.top: parent.top
            anchors.left: parent.left

            Rectangle {
                anchors.fill: parent
                gradient: pageStyle.goldButton
            }
            onClicked: {
                addingid.x = pressX + x + parent.x
                addingid.y = pressY + y + parent.y
                mainWindow.adding()
            }
        }


        Button {
            id: remove
            text: qsTr("Remove")
            width: parent.width * .45
            anchors.left: add.right
            anchors.leftMargin: (parent.width * .1)

            Rectangle {
                anchors.fill: parent
                gradient: pageStyle.goldButton
            }
            onClicked: {
                mainWindow.remove(view.model[index])
            }

        }

        Button {
            id: edit
            text: qsTr("Edit")
            width: parent.width
            anchors.top: add.bottom
            anchors.topMargin: 10
            property int index
            Rectangle {
                anchors.fill: parent
                gradient: pageStyle.goldButton
            }

            onClicked: {
                mainWindow.edit(view.model[view.currentIndex].id)
            }

        }

        Label {
            id: total
            objectName: "total"
            width: parent.width
            anchors.top: edit.bottom
            anchors.topMargin: 10
        }

    }

    ListModel {
        id: defaultModel
        objectName: "defaultModel"
    }


    TableView {
        anchors.left: cal.right
        anchors.leftMargin: 5
        implicitHeight: parent.height
        implicitWidth: parent.width * 0.2
        id: view
        objectName: "entryView"
        model: defaultModel

        property string currentType
        property int currentIndex: -1

        function fAdd(i) {
            defaultModel.append(i)
        }


        function unselectAll() {
            if(rowCount > 0) {
                selection.deselect(0, rowCount-1)
            }
            currentIndex = -1
        }

        backgroundVisible: false
        Connections {
            target: cal
            onS_datesChanged: {
                defaultModel.clear()
            }

        }

        TableViewColumn {
            role: "id"
            visible: false
        }

        TableViewColumn {
            role: "type"
            title: qsTr("[+/-]")
            width: 40
            movable: false
            resizable: false
            id: colType
        }

        TableViewColumn {
            role: "date"
            title: qsTr("Date")
            width: 100
            movable: false
        }

        TableViewColumn {
            role: "value"
            title: qsTr("Value")
            width: 100
            movable: false
        }

        TableViewColumn {
            role: "label"
            title: qsTr("Label")
            width: 100
            movable: false
        }



        itemDelegate: Rectangle {
            anchors.fill: parent
            color: "transparent"
            Label {
                id: textItem
                text: styleData.column === 2 ? Qt.formatDate(styleData.value, "dd-MM-yyyy") : (styleData.column === 1 && styleData.value === "outcome" )?  "-": (styleData.column === 1 && styleData.value === "income" )? "+" : styleData.value

                anchors.centerIn: parent
            }
        }

        rowDelegate: Rectangle {
            id:rectRow
            gradient: styleData.selected ? defaultModel.get(styleData.row).type === "outcome" ? pageStyle.selectViewOut : pageStyle.selectViewIn : pageStyle.unselectView
        }



        headerDelegate: Rectangle {
            gradient:  pageStyle.goldHeader
            height: 15
            Label {
                anchors.centerIn: parent
                text: styleData.value
            }
        }

        onClicked: {
            currentIndex = row
            currentType = defaultModel.get(row).type
        }
    }

    InformationView {
        id: infoView
        objectName: "infoView"

        width: parent.width * 0.6
        anchors.left: view.right
        anchors.leftMargin: 10
    }
}

