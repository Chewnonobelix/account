import QtQuick 2.15
import QtQuick.Controls 2.15


MouseArea {
    id: area
    signal s_datesChanged()
    signal s_monthChanged()
    property int currentMonth: month.currentMonth
    property int currentYear: month.currentYear
    property var selectedDates: month.selectedDates

    property bool multiple: true

    StackView {
        id: root
        anchors.fill: parent
        clip: true
        Component {
            id: year

            CalendarPage {
                columns: 3
                rows: 4
                model: [Date.fromLocaleString(locale, "01-01-1900", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-02-1900", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-03-1900", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-04-1900", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-05-1900", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-06-1900", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-07-1900", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-08-1900", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-09-1900", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-10-1900", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-11-1900", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-12-1900", "dd-MM-yyyy")]

                onS_select: console.log(d)
            }
        }


        Component {
            id: decade

            CalendarPage {
                columns: 5
                rows: 2
                model: [Date.fromLocaleString(locale, "01-01-1900", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-02-1901", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-03-1902", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-04-1903", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-05-1904", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-06-1905", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-07-1906", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-08-1907", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-09-1908", "dd-MM-yyyy"),
                    Date.fromLocaleString(locale, "01-10-1909", "dd-MM-yyyy")]

                onS_select: console.log(d)
            }
        }

        initialItem: MultiCalendar {
            id: month
            weekNumbersVisible: true
            multiple: area.multiple
            onS_datesChanged: area.s_datesChanged()
            onS_monthChanged: area.s_monthChanged()
        }
    }

    onWheel: {
        if(wheel.angleDelta.y < 1) {
            if(root.depth === 2) {
                root.push(decade)
            }
            if(root.depth === 1) {
                root.push(year)
            }
        }

        if(wheel.angleDelta.y > 1) {
            if(root.depth > 1) {
                root.pop()
            }
        }
    }


    acceptedButtons: Qt.NoButton
}
