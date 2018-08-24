import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4

Page {


    ListModel {
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
    }

    TableView {
        anchors.fill: parent
        id: view
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

        TableViewColumn {
            role: "labelc"
            title: qsTr("Label")
            width: 100
        }

        TableViewColumn {
            role: "total"
            title: qsTr("Total")
            width: 100
        }

        onDoubleClicked: {
            console.log(model.get(row).labelc)
        }

        model: tableModel
    }
}

