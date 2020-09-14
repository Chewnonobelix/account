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

    Connections {
        target: _pie

        function onAppend(graphName, catName, total) {
            if(graphName === "income") {
                incoming.append(catName, total)
            }
            if(graphName === "outcome") {
                outcome.append(catName, total)
            }
        }

        function onClearView() {
            incoming.clear()
            outcome.clear()
        }

    }

    Connections {
        target: _graph


        function onGranChanged(gran) {
            root.gran = gran
        }

        function onDateChanged(d) {
            root.currentDate = d
        }
    }

    GridLayout {
        anchors.fill: parent
        ChartView {
            Layout.column: 0
            Layout.row: 0
            Layout.columnSpan: 2
            title: qsTr("Income categories")
            titleFont.family: AccountStyle.title.name
            titleFont.pixelSize: AccountStyle.title.size

            backgroundColor: "transparent"
            objectName: "incomingPie"
            Layout.fillHeight: true
            Layout.fillWidth: true
            legend.font.family: AccountStyle.core.name
            legend.font.pixelSize: AccountStyle.core.size

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
            title: qsTr("Outcome categories")
            titleFont.family: AccountStyle.title.name
            titleFont.pixelSize: AccountStyle.title.size
            legend.font.family: AccountStyle.core.name
            legend.font.pixelSize: AccountStyle.core.size

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
