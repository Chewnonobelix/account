import QtQuick 2.11
import QtCharts 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.15

import Account 1.0

import "../Style"

ChartView {
    id: chart    
    
    property string granularity: qsTr("month")
    
    title: qsTr("Account evolution") + " " + granularity
    titleFont.family: AccountStyle.title.name
    titleFont.pixelSize: AccountStyle.title.size
    objectName: "chart"
    antialiasing: true
    
    property int estimatedCount: series("Estimated") ? series("Estimated").count : 0
    
    backgroundColor: "transparent"
    
    function addDataMain(td, t) {
        series("Past").append(td, t)
    }
    
    function addDataEstimated(td, t) {
        series("Estimated").append(td, t)
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
        series("Past").clear()
        series("Estimated").clear()
    }
    
    function reset () {
    }
    
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
