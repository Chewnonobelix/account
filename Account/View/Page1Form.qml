import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4

Page {

    implicitWidth: parent.width
    implicitHeight: parent.height

    id: pageTable

    property date v_date

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
    }

    function openAdding() {
        addingid.open()
        return 0
    }

    Item {
        anchors.top: cal.bottom
        width: cal.width

        Button {
            id: add
            flat: false
            text: qsTr("Add")
            width: parent.width * .45
            anchors.top: parent.top
            anchors.left: parent.left
            focus: true
            onClicked: {
                addingid.x = pressX + x + parent.x
                addingid.y = pressY + y + parent.y
                mainWindow.adding()
                console.log("Cal:" + cal.selectedDates.length)

                for(var i in cal.selectedDates) {
                    console.log(cal.selectedDates[i])
                }
            }
        }


        Button {
            id: remove
            text: qsTr("Remove")
            width: parent.width * .45
            anchors.left: add.right
            anchors.leftMargin: (parent.width * .1)

            onClicked: {
                mainWindow.remove(view.currentRow)
            }

        }

        Button {
            id: edit
            text: qsTr("Edit")
            width: parent.width
            anchors.top: add.bottom
            anchors.topMargin: 10
            property int index

            onClicked: {
                mainWindow.edit(index)
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



    TableView {
        anchors.left: cal.right
        width: parent.width - cal.width
        height: parent.height
        id: view
        objectName: "entryView"


        TableViewColumn {
            role: "date"
            title: qsTr("Date")
            width: 100
        }

        TableViewColumn {
            role: "value"
            title: qsTr("Value")
            width: 100
        }

        TableViewColumn {
            role: "label"
            title: qsTr("Label")
            width: 100
        }

        TableViewColumn {
            role: "type"
            title: qsTr("Type")
            width: 100
        }
    }
}

