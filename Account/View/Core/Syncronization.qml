import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../Style"

Item {
    id: root

    property var currentModel: null


    onCurrentModelChanged: {
        if(currentModel) {
            begin.extern(currentModel.begin)
            end.extern(currentModel.end)
        }
        console.log(currentModel)
    }

    GridLayout {
        anchors.fill: parent
        columnSpacing: root.width * 0.02
        rowSpacing: root.height * 0.02
        anchors.leftMargin: root.width * 0.02
        anchors.rightMargin: root.width * 0.02
        anchors.topMargin: root.height * 0.02
        anchors.bottomMargin: root.height * 0.02

        AccountLabel {
            Layout.row: 0
            Layout.column: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: root.height * 0.10
            Layout.preferredWidth: root.width * 0.23

            text: qsTr("Begin date")
        }

        CalendarButton {
            Layout.row: 0
            Layout.column: 1
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: root.height * 0.10
            Layout.preferredWidth: root.width * 0.22

            id: begin
            objectName: "begin"
        }

        AccountLabel {
            Layout.row: 0
            Layout.column: 2
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: root.height * 0.10
            Layout.preferredWidth: root.width * 0.23

            text: qsTr("End date")
        }

        CalendarButton {
            Layout.row: 0
            Layout.column: 3
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: root.height * 0.10
            Layout.preferredWidth: root.width * 0.22

            id: end
            objectName: "end"
        }

        AccountLabel{
            Layout.row: 1
            Layout.column: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 4
            Layout.preferredHeight: root.height * 0.10
            Layout.preferredWidth: root.width * 0.96

            property date lastSync: root.currentModel ? root.currentModel.lastSync : new Date()
            text: qsTr("Last syncronization") + ": " + Qt.formatDateTime(lastSync, "hh:mm:ss dd-MM-yyyy")
        }

        AccountHeader {
            Layout.row: 2
            Layout.column: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 4
            Layout.preferredHeight: root.height * 0.10
            Layout.preferredWidth: root.width * 0.96
            text: qsTr("Features to syncronize")
        }

        ScrollView {
            Layout.row: 3
            Layout.column: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 4
            Layout.preferredHeight: root.height * 0.60
            Layout.preferredWidth: root.width * 0.96
            Repeater {

            }
        }
    }
}
