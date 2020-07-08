import QtQuick 2.15
import QtQuick.Layouts 1.15

import "../Functionnal"
import "../Style"
import Account 1.0

MouseArea{
    acceptedButtons: Qt.NoButton
    onWheel: {
        root.s_zoom(wheel.angleDelta.y > 0 ? -1 : 1)
    }

    GridLayout {
        id: root
        objectName: "root"
        anchors.fill: parent
        rowSpacing: height * 0.02
        columnSpacing: 0

        property date currentDate: new Date()
        property var currentGran: Account.Month
        property bool okNext: true
        property bool okPrev: true
        signal s_increment(int i)
        signal s_zoom(int dir)

        AccountButton {
            id: prev
            text: "<<"

            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * 0.1
            Layout.leftMargin: root.width * 0.2

            Layout.columnSpan: 1
            Layout.column: 0
            Layout.row: 1
            Layout.alignment: Qt.AlignRight

            enabled: root.okPrev && root.currentGran !== Account.Over

            onClicked: {
                root.s_increment(-1)
            }
        }

        AccountHeader {
            property string textDate: Qt.formatDate(root.currentDate, root.currentGran === Account.Month ? "MMMM-yyyy" : root.currentGran === Account.Year ? "yyyy" : "")
            property string granText: root.currentGran === Account.Month ? qsTr("Month") : root.currentGran === Account.Year ? qsTr("Year") : qsTr("All time")
            text: textDate + " (" + granText + ")"
            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * 0.4
            Layout.alignment: Qt.AlignCenter
            Layout.columnSpan: 1
            Layout.column: 1
            Layout.row: 1
        }

        AccountButton {
            id: next
            enabled: root.okNext && root.currentGran !== Account.Over

            text: ">>"

            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * 0.1
            Layout.rightMargin: root.width * 0.2
            Layout.alignment: Qt.AlignLeft
            Layout.columnSpan: 1
            Layout.column: 2
            Layout.row: 1

            onClicked: {
                root.s_increment(1)
            }
        }

        PageView {
            id: graphSwipe
            Layout.preferredHeight: root.height * 0.90
            Layout.preferredWidth: root.width
            Layout.columnSpan: 3
            Layout.column: 0
            Layout.row: 0

            Page2Form {
                id: graph
                objectName: "timeGraph"
             }

            PieCategory {
                id: pie
                objectName: "pieCategory"
            }
        }
    }
}
