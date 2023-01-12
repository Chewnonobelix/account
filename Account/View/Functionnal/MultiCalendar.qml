import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Style"

GridLayout {
    id: root
    property bool multiple: true
    columns: 2
    Rectangle {

    }

    DayOfWeekRow {
        locale:  multiCal.locale
        Layout.fillWidth: true
        delegate: Text {
            text: narrowName
            font: control.font
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            required property string narrowName
        }
    }
    WeekNumberColumn {

    }

    MonthGrid {
        id: multiCal
        Layout.fillWidth: true

        locale: Qt.locale("en_US")
        property var selectedDates: []
        property var stylesData: []

        function addCalendar(item) {
            if(item.value !== 0) {
                calendarPreview.append(item)
            }
        }


        function addMonth(item) {
            totalPreview.append(item)
        }

        function clear() {
            calendarPreview.clear()
            totalPreview.clear()
        }

        ListModel {
            id: calendarPreview
            objectName: "calendarPreview"


            function find(day) {
                for(var i = 0; i < count; i++) {
                    if(get(i).day === day) {
                        return get(i)
                    }
                }
            }
        }

        ListModel {
            id: totalPreview
            objectName: "totalPreview"

            function find(day) {
                for(var i = 0; i < count; i++) {
                    if(get(i).day === day) {
                        return get(i)
                    }
                }
            }
        }

        readonly property string format: "dd-MM-yyyy"

        signal datesChanged()
        signal monthChanged()
        property int currentMonth
        property int currentYear
        property bool multiple: root.multiple

        month: currentMonth
        year: currentYear
        //    weekNumbersVisible: true

        signal updateSelected()

        onCurrentMonthChanged: monthChanged()

        function showNextMonth() {
            currentMonth ++

            if(currentMonth == 12) {
                showNextYear()
                currentMonth = 0
            }
            visibleMonth = Qt.binding( function() { return currentMonth})

        }


        function showPreviousMonth() {
            currentMonth --

            if(currentMonth == -1) {
                showPreviousYear()
                currentMonth = 11
            }
            month = Qt.binding( function() { return currentMonth})
        }

        function showNextYear() {
            currentYear ++

            year = Qt.binding( function() { return currentYear})

        }


        function showPreviousYear() {
            currentYear --
            year = Qt.binding( function() { return currentYear})
        }


        Component.onCompleted: {
            //        selectedDate = minimumDate
            month = new Date().getMonth()
            currentMonth = month
            year = 1900 + new Date().getYear()
            currentYear = year

            year = Qt.binding( function() { return currentYear})
            month = Qt.binding( function() { return currentMonth})
        }

        //    style: CalendarStyle {
        //        id:cs
        //        gridVisible: true
        //        gridColor: "goldenrod"

        //        background: AccountBackground {
        //            invisible: true
        //        }

        //        navigationBar: Rectangle {
        //            id: navBar
        //            height: multiCal.height/16

        //            gradient: AccountStyle.goldHeader

        //            AccountLabel {
        //                id:monthLabel
        //                height: parent.height
        //                color:"black"
        //                anchors.centerIn: parent
        //                text: Qt.locale().monthName(visibleMonth, Locale.ShortFormat) + " " + visibleYear
        //                font.family: AccountStyle.title.name
        //                font.pixelSize: height * 0.8
        //            }

        //            AccountButton {
        //                id: nextMonth
        //                anchors.right: parent.right
        //                width: multiCal.width/14
        //                height: parent.height
        //                text: ">"

        //                onClicked: {
        //                    multiCal.showNextMonth()
        //                }
        //            }

        //            AccountButton {
        //                id: prevMonth
        //                anchors.left: parent.left
        //                width: multiCal.width/14
        //                height: parent.height

        //                text: "<"

        //                onClicked: {
        //                    multiCal.showPreviousMonth()
        //                }
        //            }
    }

}
