import QtQuick 2.11
import QtQuick.Controls 2.4
import QtCharts 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4

Rectangle {
    implicitWidth: parent.width
    implicitHeight: parent.height
    id: pageChart

    AccountStyle {
        id: pageStyle
    }

        color: "transparent"

    ChartView {
        anchors.fill: parent
        anchors.centerIn: parent

        id: chart
        title: qsTr("Account evolution")
        titleFont.family: pageStyle.title.name
        titleFont.pixelSize: pageStyle.title.size
        objectName: "chart"
        antialiasing: true

        property int estimatedCount: estimatedChart.count

        backgroundColor: "transparent"

        function addDataMain(td, t) {
            series("main").append(td, t)
        }

        function addDataEstimated(td, t) {
            series("estimated").append(td, t)
        }

        function setMinMaxDate(min, max) {
            dta.max = max
            dta.min = min
        }

        function setMinMaxValue(min, max) {
            va.max = max
            va.min = min
        }

        function clear (){
            series("main").clear()
            series("estimated").clear()
        }

        function reset () {
        }

        Component.onCompleted: {
            createSeries(ChartView.SeriesTypeLine, "main", dta,va)
            createSeries(ChartView.SeriesTypeLine, "estimated", dta,va)

            series("main").objectName = "mainChart"
            series("main").pointsVisible = true

            series("estimated").objectName = "estimatedChart"
            series("estimated").pointsVisible = true

        }


        legend.font.family: pageStyle.title.name
        legend.font.pixelSize: pageStyle.title.size

        DateTimeAxis {
            id: dta
            format: "dd-MM- yyyy"
            labelsFont.family: pageStyle.core.name
            labelsFont.pixelSize: pageStyle.core.size
        }

        ValueAxis {
            id: va
            labelsFont.family: pageStyle.core.name
            labelsFont.pixelSize: pageStyle.core.size

        }
    }
}
