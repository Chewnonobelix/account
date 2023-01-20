import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Style"

GridLayout {
    id: root
    property bool multiple: true
    property var selectedDates: []
    property date selectedDate
    property int currentMonth
    property int currentYear

    rowSpacing: height * 0.01
    columns: 2

    function indexOf(list, item) {
        var ret = -1
        for(var i = 0; i < list.length && ret === -1 ; i++) {
            ret = list[i].toString() === item.toString() ? i : ret
        }

        return ret;
    }

    DayOfWeekRow {
        id: daysRow
        locale:  multiCal.locale
        Layout.fillWidth: true
        Layout.row: 0
        spacing: 0
        Layout.column: 1
        Layout.alignment: Qt.AlignBottom
        delegate: AccountHeader {
            text: narrowName

            required property string narrowName
        }
    }
    WeekNumberColumn {
        month: multiCal.month
        year: multiCal.year
        Layout.alignment: Qt.AlignTop
        Layout.fillHeight: true
        spacing: 0

        delegate: AccountHeader {
            text: weekNumber
            required property int weekNumber
        }
    }

    MonthGrid {
        id: multiCal
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.alignment: Qt.AlignTop
        spacing: 0
        locale: Qt.locale("fr_FR")

        month: Math.max(root.currentMonth - 1, 0)
        year: root.currentYear
        delegate: Text {
            id: dayDel
            property bool isCurrentMonth: model.month === multiCal.month
            Rectangle {
                z: -1
                anchors.fill: parent
                border.color: "black"
                property bool isSelected: indexOf(selectedDates, model.date) !== -1
                gradient: !isSelected ?  AccountStyle.backgroundGradient : AccountStyle.calSelect

                MouseArea {
                    onClicked: (mouse) => {
                                   if(dayDel.isCurrentMonth) {
                                       if(!parent.isSelected) {
                                           if(!(root.multiple && mouse.modifiers === Qt.ShiftModifier))
                                                selectedDates.splice(0, selectedDates.length)
                                           selectedDates.push(model.date)
                                       }
                                       else {
                                           selectedDates.splice(indexOf(selectedDates, model.date), 1)
                                       }

                                       root.selectedDatesChanged()
                                   }
                               }

                    anchors.fill: parent
                }
            }


            text: model.day
            color: isCurrentMonth ? "black" : "grey"
            fontSizeMode: Text.Fit
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
        }
    }
}
