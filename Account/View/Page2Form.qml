import QtQuick 2.11
import QtQuick.Controls 2.4
import QtCharts 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4

Page {
    implicitWidth: parent.width
    implicitHeight: parent.height
    //    id: page2

    //    header: Label {
    //        text: qsTr("Page 2")
    //        font.pixelSize: Qt.application.font.pixelSize * 2
    //        padding: 10
    //    }

    ChartView {
        anchors.fill: parent
        anchors.centerIn: parent
        //theme: ChartView.ChartThemeBlueIcy
        id: chart
        title: "Chart"
        objectName: "chart"
        antialiasing: true

        backgroundColor: "transparent"

        function addData(/*md, m, ed, e,*/ td, t) {
//            mainChart.append(md, m)
//            estimatedChart.append(ed, t)
            totalChart.append(td, t)

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
            totalChart.clear()
        }

        function reset () {
            setAxisX(dta, mainChart)
            setAxisX(dta, estimatedChart)
            setAxisX(dta, totalChart)
            setAxisY(va, mainChart)
            setAxisY(va, estimatedChart)
            setAxisY(va, totalChart)
        }

        Component.onCompleted: {
            reset()
        }

        DateTimeAxis {
            id: dta
        }

        ValueAxis {
            id: va
        }

        LineSeries {
//            color: "transparent"
            pointsVisible: true
            name: "main"
            id: mainChart
            objectName: "mainChart"
        }

        LineSeries {
            pointsVisible: true
            name: "estimated"
            id: estimatedChart
            objectName: "estimatedChart"
        }

        LineSeries{
            pointsVisible: true
            name: "total"
            id: totalChart
            objectName: "totalChart"
        }
    }
}
