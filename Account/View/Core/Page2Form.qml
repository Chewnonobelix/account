import QtQuick 2.11
import QtCharts 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4
import QtQuick.Controls 2.4
import "../Style"

Rectangle {
    implicitWidth: parent.width
    implicitHeight: parent.height
    id: pageChart
    
    
    color: "transparent"
    
    property int month
    property int year
    
    Button {
        id: next
        enabled: chart.okNext
        
        anchors.left: chart.right
        anchors.verticalCenter: chart.verticalCenter
        text: qsTr(">")
        height: chart.height
        width: parent.width * 0.025
        
        background: Rectangle {
            anchors.fill: parent
            gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
        }
        
        onClicked: {
            chart.s_increment(1)
        }
    }
    
    Button {
        id: prev
        anchors.right: chart.left
        anchors.verticalCenter: chart.verticalCenter
        text: qsTr("<")
        height: chart.height
        width: parent.width * 0.025
        
        enabled: chart.okPrev
        
        background: Rectangle {
            anchors.fill: parent
            gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
        }
        
        onClicked: {
            chart.s_increment(-1)
        }
    }
    
    ChartView {
        anchors.centerIn: parent
        
        height: parent.height
        width: parent.width * .95
        id: chart
        
        property bool okNext: true
        property bool okPrev: true
        
        MouseArea {
            acceptedButtons: Qt.NoButton
            anchors.fill: parent
            
            onWheel: {
                parent.s_zoom(wheel.angleDelta.y > 0 ? -1 : 1)
            }
        }
        
        property int month: parent.month
        property int years: parent.year
        
        signal s_increment(int i)
        signal s_zoom(int dir)
        
        property string granularity: qsTr("month")
        
        title: qsTr("Account evolution") + " " + granularity
        titleFont.family: AccountStyle.title.name
        titleFont.pixelSize: AccountStyle.title.size
        objectName: "chart"
        antialiasing: true
        
        property int estimatedCount: series(qsTr("estimated")) ? series(qsTr("estimated")).count : 0
        
        backgroundColor: "transparent"
        
        function addDataMain(td, t) {
            series(qsTr("past")).append(td, t)
        }
        
        function addDataEstimated(td, t) {
            series(qsTr("estimated")).append(td, t)
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
            series(qsTr("past")).clear()
            series(qsTr("estimated")).clear()
        }
        
        function reset () {
        }
        
        Component.onCompleted: {
            createSeries(ChartView.SeriesTypeLine, qsTr("past"), dta,va)
            createSeries(ChartView.SeriesTypeLine, qsTr("estimated"), dta,va)
            
            series(qsTr("past")).objectName = "mainChart"
            series(qsTr("past")).pointsVisible = true
            
            series(qsTr("estimated")).objectName = "estimatedChart"
            series(qsTr("estimated")).pointsVisible = true
            
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
