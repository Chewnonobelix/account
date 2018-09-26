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
                id: rectAdd
                anchors.fill: parent
                gradient: pageStyle.goldButton
            }

            onPressed: {
                rectAdd.gradient = pageStyle.darkGoldButton
            }

            onReleased: {
                rectAdd.gradient = pageStyle.goldButton
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
                id: rectRemove

                anchors.fill: parent
                gradient: pageStyle.goldButton
            }

            onPressed: {
                rectRemove.gradient = pageStyle.darkGoldButton
            }

            onReleased: {
                rectRemove.gradient = pageStyle.goldButton
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
                id: rectEdit
                anchors.fill: parent
                gradient: pageStyle.goldButton
            }

//            onClicked: {
//                mainWindow.edit(view.model[view.currentIndex].id)
//            }

            onPressed: {
                rectEdit.gradient = pageStyle.darkGoldButton
            }

            onReleased: {
                rectEdit.gradient = pageStyle.goldButton
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

        function reset() {
            defaultModel.clear()
        }

        backgroundVisible: false
        Connections {
            target: cal
            onS_datesChanged: {
                view.reset()
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

        }

        TableViewColumn {
            role: "date"
            title: qsTr("Date")
            width: 100
            movable: false
            resizable: false

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


        itemDelegate: Rectangle {
            width: parent.width
            height: 20
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

            width: parent.width
            height: 20

            gradient: styleData.selected ? defaultModel.get(styleData.row).type === "outcome" ? pageStyle.selectViewOut : pageStyle.selectViewIn : pageStyle.unselectView
        }


        style: TableViewStyle {
            headerDelegate: Rectangle {
                height: headerText.implicitHeight * 1.2
                width: headerText.implicitWidth
                gradient:  pageStyle.goldHeader
                Label {
                    id: headerText
                    anchors.centerIn: parent
                    text: styleData.value
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        console.log("Sort " + styleData)
                    }
                }
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

//        width: parent.width * 0.6
        anchors.left: view.right
        anchors.right: parent.right
        anchors.leftMargin: 10

    }
}

