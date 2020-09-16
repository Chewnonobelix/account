import QtQuick 2.15
import QtQuick.Layouts 1.15

import "../Style"
import "../Core"

Item {
    id: root

    GridLayout {
        anchors {
            fill: parent
            topMargin: root.height * 0.02
            bottomMargin: root.height * 0.02
            leftMargin: root.width * 0.02
            rightMargin: root.width * 0.02
        }

        columnSpacing: root.width * 0.02
        rowSpacing: root.height * 0.02

        ListView {
            Layout.column: 0
            Layout.row: 0
            Layout.columnSpan: 2
            Layout.rowSpan: 9
            Layout.preferredHeight: root.height *0.88
            Layout.preferredWidth: root.width *0.20
        }

        AccountButton {
            Layout.column: 0
            Layout.row: 9
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.06
            Layout.preferredWidth: root.width *0.09

            text: qsTr("Add")
        }

        AccountButton {
            Layout.column: 1
            Layout.row: 9
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.06
            Layout.preferredWidth: root.width *0.09

            text: qsTr("Remove")
        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 0
            Layout.columnSpan: 4
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.05
            Layout.preferredWidth: root.width *0.74

            text: qsTr("Name")
        }

        AccountTextInput {
            Layout.column: 2
            Layout.row: 1
            Layout.columnSpan: 4
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.05
            Layout.preferredWidth: root.width *0.74

        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 2
            Layout.columnSpan: 4
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.05
            Layout.preferredWidth: root.width *0.74

            text: qsTr("Reference")
        }

        EntryEdit {
            Layout.column: 2
            Layout.row: 3
            Layout.columnSpan: 4
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.22
            Layout.preferredWidth: root.width *0.74

        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 4
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.08

            text: qsTr("Date")
        }

        CalendarButton {
            Layout.column: 3
            Layout.row: 4
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.10

        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 5
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.08

            text: qsTr("Type")
        }

        AccountComboBox {
            Layout.column: 3
            Layout.row: 5
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.10

        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 6
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.08

            text: qsTr("Rate")
        }

        AccountSpinbox {
            Layout.column: 3
            Layout.row: 6
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.10

        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 7
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.08

            text: qsTr("Time")
        }

        AccountSpinbox {
            Layout.column: 3
            Layout.row: 7
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.10

        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 8
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.08

            text: qsTr("Frequency")
        }

        AccountComboBox {
            Layout.column: 3
            Layout.row: 8
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.10

        }

        AccountButton {
            Layout.column: 2
            Layout.row: 9
            Layout.columnSpan: 2
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.06
            Layout.preferredWidth: root.width *0.20

            text: qsTr("Generate")
        }

        ListView {
            Layout.column: 4
            Layout.row: 4
            Layout.columnSpan: 1
            Layout.rowSpan: 6
            Layout.preferredHeight: root.height *0.53
            Layout.preferredWidth: root.width *0.25

        }

        EntryText {
            Layout.column: 5
            Layout.row: 4
            Layout.columnSpan: 1
            Layout.rowSpan: 6
            Layout.preferredHeight: root.height *0.53
            Layout.preferredWidth: root.width *0.25

        }
    }
}
