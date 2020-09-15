import QtQuick 2.15
import QtQuick.Layouts 1.15

import "../Style"
import "../Core"

Item {
    id: root

    GridLayout {
        anchors.fill: parent
        columnSpacing: root.width * 0.02
        rowSpacing: root.height * 0.02

        ListView {
            Layout.column: 0
            Layout.row: 0
            Layout.columnSpan: 2
            Layout.rowSpan: 6
            Layout.preferredHeight: root.height *0.89
            Layout.preferredWidth: root.width *0.20
        }

        AccountButton {
            Layout.column: 0
            Layout.row: 6
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.05
            Layout.preferredWidth: root.width *0.09

            text: qsTr("Add")
        }

        AccountButton {
            Layout.column: 1
            Layout.row: 6
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.05
            Layout.preferredWidth: root.width *0.09

            text: qsTr("Remove")
        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 0
            Layout.columnSpan: 5
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.05
            Layout.preferredWidth: root.width *0.30

            text: qsTr("Name")
        }

        AccountTextInput {
            Layout.column: 2
            Layout.row: 1
            Layout.columnSpan: 5
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.05
            Layout.preferredWidth: root.width *0.30

        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 2
            Layout.columnSpan: 5
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.05
            Layout.preferredWidth: root.width *0.30

        }

    }
}
