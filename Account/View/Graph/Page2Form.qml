import QtQuick 2.15
import QtCharts 2.2
//import QtQuick.Controls.Styles 1.4
//import QtQuick.Controls 1.4
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.15

import Account 1.0

import "../Style"


Rectangle {
    id: root
    property string granularity: qsTr("month")
    property int estimatedCount: chart.series("Estimated") ? chart.series("Estimated").count : 0

    Connections {
        target: _timeGraph

        function onAppendMain(td, t) {
            chart.series("Past").append(td, t)
        }

        function onAppendEstimated(td, t) {
            chart.series("Estimated").append(td, t)
        }

        function onDateChanged(min, max) {
            dta.max = max
            dta.min = min
        }

        function onBornValueChanged(min, max) {
            va.max = max
            va.min = min
        }

        function onClearView (){
            chart.series("Past").clear()
            chart.series("Estimated").clear()
        }
    }


    AccountBackground {
        anchors.fill: parent
    }

    ChartView {
        id: chart

        title: qsTr("Account evolution") + " " + granularity
        titleFont.family: AccountStyle.title.name
        titleFont.pixelSize: AccountStyle.title.size
        objectName: "chart"
        antialiasing: true
        anchors.fill: parent

        backgroundColor: "transparent"

        Component.onCompleted: {
            createSeries(ChartView.SeriesTypeLine, "past", dta,va)
            createSeries(ChartView.SeriesTypeLine, "estimated", dta,va)

            series("past").objectName = "mainChart"
            series("past").pointsVisible = true
            series("past").name = qsTr("Past")
            series("estimated").objectName = "estimatedChart"
            series("estimated").pointsVisible = true
            series("estimated").name = qsTr("Estimated")
        }


        legend.font.family: AccountStyle.title.name
        legend.font.pixelSize: AccountStyle.title.size

        DateTimeAxis {
            id: dta
            format: "dd-MM- yyyy"
            labelsFont.family: AccountStyle.core.name
            labelsFont.pixelSize: AccountStyle.core.size
        }

        ValueAxis {
            id: va
            labelsFont.family: AccountStyle.core.name
            labelsFont.pixelSize: AccountStyle.core.size

        }
    }
}
