import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Style"

Item {
    id: root

    property string label: "Ok boomer"
    property int columns
    property int rows
    property var model:[]
    property bool decade: false
    
    signal s_select(date d)
    signal showPrevious()
    signal showNext()
    
    GridLayout {
        anchors.fill: parent

        rowSpacing: 0
        columnSpacing: 0

        AccountButton {
            text: "<"

            Layout.column: 0
            Layout.row: 0
            Layout.preferredHeight: root.height / 16
            Layout.preferredWidth: root.width / 14
            onClicked: showPrevious()
        }
        AccountHeader {
            text: root.label
            Layout.column: 1
            Layout.row: 0
            Layout.preferredHeight: root.height / 16
            Layout.fillWidth: true
        }
        AccountButton {
            text: ">"
            Layout.column: 2
            Layout.row: 0
            Layout.preferredHeight: root.height / 16
            Layout.preferredWidth: root.width / 14
            onClicked: showNext()
        }

        Grid {
            Layout.column: 0
            Layout.row: 1
            Layout.preferredHeight: root.height * 0.95
            Layout.preferredWidth: root.width
            Layout.columnSpan: 3

            columns: root.columns
            rows: root.rows

            id: grid
            Repeater {
                model: root.model

                AccountBackground {
                    width: grid.width / grid.columns
                    height: grid.height / grid.rows

                    AccountLabel {
                        anchors.fill: parent
                        text: Qt.formatDate(modelData, decade ? "yyyy" : "MMM")

                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: root.s_select(modelData)
                        cursorShape: Qt.PointingHandCursor
                    }
                }
            }


        }
    }
}
