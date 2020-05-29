import QtQuick 2.11
import QtCharts 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.15
import Account 1.0

import "../Style"

Rectangle {
    id: root

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.NoButton
        onWheel: root.s_zoom(wheel.angleDelta.y > 0 ? -1 : 1)
    }

    property date currentDate: new Date()
    property var gran: Account.Month

    AccountBackground {
        invisible: false
        anchors.fill: parent

    }

    signal s_increment(int i)
    signal s_zoom(int dir)

    GridLayout {
        anchors.fill: parent
        ChartView {
            Layout.column: 0
            Layout.row: 0
            Layout.columnSpan: 2

            backgroundColor: "transparent"
            objectName: "incomingPie"
            Layout.fillHeight: true
            Layout.fillWidth: true

            function clear() {
                incoming.clear()
            }

            function append(name, value) {
                incoming.append(name, value)
            }

            ToolTip {
                id: tooli
            }

            PieSeries {
                name: "incoming"
                id: incoming

                onHovered: {
                    slice.exploded = state
                    slice.labelVisible = state
                    tooli.visible = state
                    tooli.text = (slice.percentage*100).toFixed(2) + "%"
                }
            }
        }

        ChartView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            backgroundColor: "transparent"
            objectName: "outcomePie"
            Layout.column: 2
            Layout.row: 0
            Layout.columnSpan: 2

            function clear() {
                outcome.clear()
            }

            function append(name, value) {
                outcome.append(name, value)
            }

            ToolTip {
                id: toolo
            }

            PieSeries {
                id: outcome
                name: "outcome"

                onHovered: {
                    slice.exploded = state
                    slice.labelVisible = state
                    toolo.visible = state
                    toolo.text = (slice.percentage*100).toFixed(2) + "%"
                }
            }
        }

        AccountButton {
            id: prev
            text: qsTr("<<")

            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * 0.2
            Layout.columnSpan: 1
            Layout.column: 0
            Layout.row: 1

//            enabled: chart.okPrev

            onClicked: {
                root.s_increment(-1)
            }
        }

        AccountLabel {
            text: Qt.formatDate(root.currentDate, root.gran === Account.Month ? "MMMM-yyyy" : root.gran === Account.Year ? "yyyy" : "")
            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * 0.4
            Layout.columnSpan: 2
            Layout.column: 1
            Layout.row: 1
            font.family: AccountStyle.title.name
            font.pixelSize: AccountStyle.title.size
        }

        AccountButton {
            id: next
            text: qsTr(">>")

            Layout.preferredHeight: root.height * 0.05
            Layout.preferredWidth: root.width * 0.2
            Layout.columnSpan: 1
            Layout.column: 3
            Layout.row: 1

//            enabled: chart.okPrev

            onClicked: {
                root.s_increment(-1)
            }
        }

    }
}
