import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4

Page {

    header: Label {
        text: qsTr("Account")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    TableView {
        anchors.fill: parent
        TableViewColumn {
            role: "Date"
            title: qsTr("Date")
            width: 100
        }

        TableViewColumn {
            role: "Value"
            title: qsTr("Value")
            width: 100
        }

        TableViewColumn {
            role: "Label"
            title: qsTr("Label")
            width: 100
        }

        TableViewColumn {
            role: "Total"
            title: qsTr("Total")
            width: 100
        }
    }
}
