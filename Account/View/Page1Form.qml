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
                console.log(index)
                mainWindow.edit(index)
            }

        }
    }

    /*ListModel {
        id: tableModel

        ListElement {
            datec: "2018-11-6"
            value: 25.3
            labelc: "X text"
            total: 65.6
        }

        ListElement {
            datec: "2018-5-6"
            value: 465
            labelc: "y text"
            total: 45.6
        }
    }*/

    property var myList: []

    function updateModel () {
        myModel.clear()
        for(var i in myList) {
            console.log(myList[i].value + " i")
        }
    }

    ListModel {
        id: myModel
        objectName: "myModel"
    }

    Component {
        id: tabViewDelegate

        Text {
            text: date
        }
    }

    TableView {
        anchors.left: cal.right
        width: parent.width - cal.width
        height: parent.height
        id: view
        objectName: "entryView"

        model: myModel

        TableViewColumn {
            role: "datec"
            title: qsTr("Date")
            width: 100
        }

        TableViewColumn {
            role: "value"
            title: qsTr("Value")
            width: 100
        }

        /*TableViewColumn {
            role: "labelc"
            title: qsTr("Label")
            width: 100
        }*/

        TableViewColumn {
            role: "total"
            title: qsTr("Total")
            width: 100
        }

//        onClicked: {
//            edit.index = row
//        }


//        onDoubleClicked: {
//            console.log(model.get(row).labelc)
//            clicked(row)
//            edit.clicked()
//        }

        onModelChanged: {
            console.log("Model changed " + model.count)
        }

//        itemDelegate: tabViewDelegate
    }
}

