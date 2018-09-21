import QtQuick 2.11
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Calendar {
    id: multiCal

    property var selectedDates: []
    property var stylesData: []

    signal s_datesChanged()
    readonly property string format: "dd-MM-yyyy"
    property int currentMonth
    property int currentYear

    visibleMonth: currentMonth
    visibleYear: currentYear


    function showNextMonth() {
        currentMonth ++

        if(currentMonth == 12) {
            showNextYear()
            currentMonth = 0
        }
        visibleMonth = currentMonth

    }


    function showPreviousMonth() {
        currentMonth --
        visibleMonth = currentMonth

        if(currentMonth == -1) {
            showPreviousYear()
            currentMonth = 11
        }
        visibleMonth = currentMonth
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
        background: Rectangle {
            color: "transparent"
        }

        navigationBar: Rectangle {
            height: multiCal.height/16

            gradient: buttonMonth

            Label {
                id:monthLabel
                color:"black"
                anchors.centerIn: parent
                text: Qt.locale().monthName(visibleMonth, Locale.ShortFormat) + " " + visibleYear
            }

            Button {
                id: nextMonth
                anchors.right: parent.right
                width: multiCal.width/14
                height: parent.height
                style: ButtonStyle {
                    background: Rectangle {
                        gradient: buttonMonth
                        border.color: "silver"

                    }
                }

                text: ">"
                onClicked: {
                    multiCal.showNextMonth()
                }
            }

            Button {
                id: prevMonth
                anchors.left: parent.left
                width: multiCal.width/14
                height: parent.height
                style: ButtonStyle {
                    background: Rectangle {
                        gradient: buttonMonth
                        border.color: "silver"
                    }
                }

                text: "<"
                onClicked: {
                    multiCal.showPreviousMonth()
                }
            }

        }


        Gradient {
            id: gradientSelect

            GradientStop {
                color: "goldenrod"
                position: 0.0
            }

            GradientStop {
                color: "gold"
                position: 0.5
            }

            GradientStop {
                color: "goldenrod"
                position: 1.0
            }
        }

        Gradient {
            id: gradientUnSelect
            GradientStop {
                color: "transparent"
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
            gradient: buttonMonth
            width: multiCal.width/14
            anchors.top: navigationBar.bottom

            border.color: "darkgoldenrod"
            Label {
                anchors.centerIn: parent
                text: styleData.weekNumber
            }
        }

        dayOfWeekDelegate: Rectangle {
            anchors.centerIn: parent
            anchors.top: navigationBar.bottom
            gradient: buttonMonth
            height: multiCal.height/16
            border.color: "darkgoldenrod"

            Label {
                anchors.centerIn: parent
                text: Qt.locale().dayName(styleData.dayOfWeek, Locale.ShortFormat)
            }
        }
        dayDelegate: Rectangle {
            signal updateSelected()
            signal reset()
            id: styleRect

            onReset: {
                view.unselectAll()

                for(var i in stylesData) {
                    stylesData[i][0].color = (stylesData[i][1].date.getMonth() === (visibleMonth)) ? "black" : "grey"
                    stylesData[i][0].parent.gradient = gradientUnSelect
                }

                while(selectedDates.length > 0) {
                    selectedDates.pop()
                }
            }

            onUpdateSelected: {
                view.unselectAll()

                if(cs.isSelected(styleData) && (styleData.date.getMonth() === visibleMonth)) {
                    styleRect.gradient = gradientSelect
                }
                else {
                    styleRect.gradient = gradientUnSelect
                }

                visibleMonth = currentMonth
                visibleYear = currentYear

                multiCal.s_datesChanged()
            }

            Label {
                id: c_date
                anchors.centerIn: parent
                text: styleData.date.getDate()
                color: styleData.date.getMonth() === (multiCal.visibleMonth) ? "black" : "grey"
                onTextChanged: {

                    if(stylesData.indexOf(c_date) == -1) {
                        stylesData.push([c_date, styleData])
                    }
                    reset()
                }
            }

            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                onClicked: {
                    if(mouse.modifiers === Qt.ShiftModifier) {
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
                    else {
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

    Gradient {
        id: buttonMonth
        GradientStop {
            color: "darkgoldenrod"
            position: 0.0
        }
        GradientStop {
            color: "gold"
            position: 0.5
        }
        GradientStop {
            color: "darkgoldenrod"
            position: 1.0
        }
    }
}


