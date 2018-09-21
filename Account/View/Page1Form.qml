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
            }
        }


        Button {
            id: remove
            text: qsTr("Remove")
            width: parent.width * .45
            anchors.left: add.right
            anchors.leftMargin: (parent.width * .1)

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
        width: parent.width - cal.width
        height: parent.height
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

//        TableViewColumn {
//            role: "type"
//            title: qsTr("Type")
//            width: 100
//        }

        itemDelegate: Label {
            id: textItem
            text: styleData.column === 1 ? Qt.formatDate(styleData.value, "dd-MM-yyyy") : styleData.value


        }

        rowDelegate: Rectangle {
            id:rectRow
            gradient: styleData.selected ? defaultModel.get(styleData.row).type === "outcome" ? selectViewOut : selectViewIn : unselectView
        }


    style: TableViewStyle{
        highlightedTextColor: "gold"
        activateItemOnSingleClick: true
    }

    onClicked: {
        currentIndex = row
        currentType = defaultModel.get(row).type
    }

    Gradient {
        id:selectViewOut
        GradientStop {
            color: "gold"
            position: 0.0
        }
        GradientStop {
            color: "darkred"
            position: 0.5
        }
        GradientStop {
            color: "gold"
            position: 1.0        }
    }

    Gradient {
        id:selectViewIn
        GradientStop {
            color: "gold"
            position: 0.0
        }
        GradientStop {
            color: "aquamarine"
            position: 0.5
        }
        GradientStop {
            color: "gold"
            position: 1.0        }
    }

    Gradient {
        id: unselectView
        GradientStop {
            color: "transparent"
        }
    }
}
}

