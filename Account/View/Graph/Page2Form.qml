import QtQuick 2.11
import QtCharts 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.15

import Account 1.0

import "../Style"

GridLayout {
    //    implicitWidth: parent.width
    //    implicitHeight: parent.height
    id: pageChart
    
    
    
    rowSpacing: pageChart.height * .02
    columnSpacing: pageChart.width * .02
    ChartView {
        id: chart
        
        Layout.preferredHeight: pageChart.height * 0.93
        Layout.fillWidth: true
        Layout.columnSpan: 3
        
        property bool okNext: true
        property bool okPrev: true
        
        MouseArea {
            acceptedButtons: Qt.NoButton
            anchors.fill: parent
            
            onWheel: {
                parent.s_zoom(wheel.angleDelta.y > 0 ? -1 : 1)
            }
        }
        
        property date currentDate: new Date()
        property var currentGran: Account.Month
        signal s_increment(int i)
        signal s_zoom(int dir)
        
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
    
    
    AccountButton {
        id: prev
        text: qsTr("<<")
        
        Layout.preferredHeight: pageChart.height * 0.05
        Layout.preferredWidth: pageChart.width * 0.2
        Layout.columnSpan: 1
        Layout.column: 0            
        Layout.row: 1 
        
        enabled: chart.okPrev
        
        onClicked: {
            chart.s_increment(-1)
        }
    }
    
    AccountLabel {
        text: Qt.formatDate(chart.currentDate, chart.currentGran === Account.Month ? "MMMM-yyyy" : chart.currentGran === Account.Year ? "yyyy" : "")
        Layout.preferredHeight: pageChart.height * 0.05
        Layout.preferredWidth: pageChart.width * 0.4
        Layout.columnSpan: 1
        Layout.column: 1            
        Layout.row: 1 
        font.family: AccountStyle.title.name
        font.pixelSize: AccountStyle.title.size
        
    }
    
    AccountButton {
        id: next
        enabled: chart.okNext
        
        text: qsTr(">>")
        
        Layout.preferredHeight: pageChart.height * 0.05
        Layout.preferredWidth: pageChart.width * 0.1
        Layout.alignment: Qt.AlignLeft
        Layout.columnSpan: 1
        Layout.column: 2            
        Layout.row: 1 
        
        
        onClicked: {
            chart.s_increment(1)
        }
    }
}
