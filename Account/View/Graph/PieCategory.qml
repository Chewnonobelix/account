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
    }
}
