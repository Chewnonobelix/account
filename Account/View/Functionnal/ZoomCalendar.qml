import QtQuick 2.15
import QtQuick.Controls 2.15


MouseArea {
    signal s_datesChanged()
    signal s_monthChanged()
    property int currentMonth: month.currentMonth
    property int currentYear: month.currentYear
    property var selectedDates: month.selectedDates

    property bool multiple: true
    id: area
    StackView {
        id: root
        anchors.fill: parent

        Component {
            id: year

            Label {
                text: "year"
            }
        }

        Component {
            id: decade

            Label {
                text: "decade"
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
