import QtQuick 2.11
import QtQuick.Controls 1.4

import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import "../Style"

Calendar {
    id: multiCal

    property var selectedDates: []
    property var stylesData: []

    ListModel {
        id: calendarPreview
        objectName: "calendarPreview"

        function add(item) {
            if(item.value !== 0) {
                append(item)
            }
        }

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

        function add(item) {
            append(item)
        }

        function find(day) {
            for(var i = 0; i < count; i++) {
                if(get(i).day === day) {
                    return get(i)
                }
            }
        }
    }

    signal s_datesChanged()
    signal s_monthChanged()
    readonly property string format: "dd-MM-yyyy"
    property int currentMonth
    property int currentYear

    visibleMonth: currentMonth
    visibleYear: currentYear
    weekNumbersVisible: true

    MouseArea {
        z: -1
        anchors.fill: parent
        acceptedButtons: Qt.NoButton
        onWheel: console.log(wheel.angleDelta)
    }

    function showNextMonth() {
        currentMonth ++

        if(currentMonth == 12) {
            showNextYear()
            currentMonth = 0
        }
        visibleMonth = currentMonth

        s_monthChanged()
    }


    function showPreviousMonth() {
        currentMonth --
        visibleMonth = currentMonth

        if(currentMonth == -1) {
            showPreviousYear()
            currentMonth = 11
        }
        visibleMonth = currentMonth
        s_monthChanged()
    }

    function showNextYear() {
        currentYear ++

        visibleYear = currentYear

    }


    function showPreviousYear() {
        currentYear --
        visibleYear = currentYear

        visibleYear = currentYear
    }


    Component.onCompleted: {
        selectedDate = minimumDate
        visibleMonth = new Date().getMonth()
        currentMonth = visibleMonth
        visibleYear = 1900 + new Date().getYear()
        currentYear = visibleYear
    }

    style: CalendarStyle {
        id:cs
        gridVisible: true
        gridColor: "goldenrod"

        background: AccountBackground {
            invisible: true
        }

        navigationBar: Rectangle {
            id: navBar
            height: multiCal.height/16

            gradient: AccountStyle.goldHeader

            AccountLabel {
                id:monthLabel
                height: parent.height
                color:"black"
                anchors.centerIn: parent
                text: Qt.locale().monthName(visibleMonth, Locale.ShortFormat) + " " + visibleYear
                font.family: AccountStyle.title.name
                font.pixelSize: height * 0.8
            }

            AccountButton {
                id: nextMonth
                anchors.right: parent.right
                width: multiCal.width/14
                height: parent.height
                text: ">"

                onClicked: {
                    multiCal.showNextMonth()
                }
            }

            AccountButton {
                id: prevMonth
                anchors.left: parent.left
                width: multiCal.width/14
                height: parent.height

                text: "<"

                onClicked: {
                    multiCal.showPreviousMonth()
                }
            }
        }

        function isSelected(sd) {
            var ret = false

            for(var i in selectedDates) {
                if(Qt.formatDate(sd.date, format) === selectedDates[i]) {
                    ret = true
                }
            }
            return ret
        }

        weekNumberDelegate: Rectangle {
            gradient: AccountStyle.goldHeader
            width: multiCal.width/14
            height: multiCal.height/7
            anchors.top: navigationBar.bottom

            border.color: "darkgoldenrod"
            AccountLabel {
                id: wndLabel
                height: parent.height
                anchors.centerIn: parent
                text: styleData.weekNumber
                font.pixelSize: height * 0.3
            }
        }

        dayOfWeekDelegate: Rectangle {
            anchors.centerIn: parent
            anchors.top: navigationBar.bottom
            gradient: AccountStyle.goldHeader
            height: multiCal.height/16
            border.color: "darkgoldenrod"

            AccountLabel {
                anchors.centerIn: parent
                height: parent.height
                font.pixelSize: height * 0.55
                text: Qt.locale().dayName(styleData.dayOfWeek, Locale.ShortFormat)
            }
        }
        dayDelegate: Rectangle {
            signal updateSelected()
            signal reset()

            property int day: Qt.formatDate(styleData.date, "d")
            property double val: delCurrentMonth && calendarPreview.find(day) !== undefined ? calendarPreview.find(day).value : 0
            property bool delCurrentMonth: styleData.date.getMonth() === visibleMonth

            id: styleRect
            gradient: blueGradient
            onReset: {
                view.unselectAll()

                for(var i in stylesData) {
                    stylesData[i][0].color = (stylesData[i][1].date.getMonth() === (visibleMonth)) ? "black" : "grey"
                    stylesData[i][0].parent.gradient = AccountStyle.backgroundGradient
                }

                while(selectedDates.length > 0) {
                    selectedDates.pop()
                }
            }

            ToolTip {
                text: qsTr("Total") +": " + totalRect.total + "€ \n" + qsTr("Day transaction") +": " + parent.val +"€"
                visible: styleData.hovered && parent.delCurrentMonth
                delay: 500
            }

            Rectangle {
                width: parent.width
                height: parent.height * .05
                anchors.topMargin: 2
                anchors.top: parent.top
                property double total: isFind ? calendarPreview.find(parent.day).value : 0
                property bool isFind: calendarPreview.find(parent.day) !== undefined

                color: {
                    if(isFind && parent.delCurrentMonth) {
                        color: total > 0 ? "green" : "red"
                    }
                    else {
                        color: "transparent"
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: parent.height * .05
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 2
                id: totalRect
                property double total: isFind ? totalPreview.find(parent.day).value : 0
                property bool isFind: totalPreview.find(parent.day) !== undefined
                color: {
                        color:  parent.delCurrentMonth && isFind ? total > 0 ? "green" : "red" : "transparent"
                }
            }

            onUpdateSelected: {
                view.unselectAll()

                if(cs.isSelected(styleData) && checkMonth(styleData)) {
                    styleRect.gradient = AccountStyle.calSelect
                }
                else if(checkMonth(styleData)){
                    styleRect.gradient = AccountStyle.unselectView
                }

                visibleMonth = currentMonth
                visibleYear = currentYear

                multiCal.s_datesChanged()
            }

            AccountLabel {
                id: c_date
                anchors.centerIn: parent
                text: styleData.date.getDate()

                color:  parent.delCurrentMonth ? "black" : "grey"
                onTextChanged: {

                    if(stylesData.indexOf(c_date) == -1) {
                        stylesData.push([c_date, styleData])
                    }
                    reset()
                }


            }

            function checkMonth(m) {
                return m.date.getMonth() === (multiCal.visibleMonth)
            }

            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                cursorShape: parent.checkMonth(styleData) ? Qt.PointingHandCursor: Qt.ArrowCursor

                onClicked: {
                    if((mouse.modifiers === Qt.ShiftModifier) && parent.checkMonth(styleData)){
                        var index = -1
                        for(var i in selectedDates) {
                            if(Qt.formatDate(styleData.date, format) === selectedDates[i]) {
                                index = i
                            }

                        }
                        if( index == -1) {
                            selectedDates[selectedDates.length] = Qt.formatDate(styleData.date, format)
                        }
                        else {
                            selectedDates.splice(index,1)
                        }
                    }
                    else if(parent.checkMonth(styleData)){
                        parent.reset()
                        if(!styleData.selected && Qt.formatDate(selectedDate, format) !== Qt.formatDate(styleData.date, format) && styleData.date.getMonth() === (multiCal.visibleMonth)) {
                            selectedDates[selectedDates.length] = Qt.formatDate(styleData.date, format)
                            selectedDate = styleData.date
                        }
                        else {
                            selectedDates.splice(0,1)
                            selectedDate = minimumDate
                        }
                    }
                    parent.updateSelected()
                }
            }
        }
    }
}


