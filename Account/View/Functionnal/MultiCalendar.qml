import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Style"

GridLayout {
    id: root
    property bool multiple: true
    columns: 2
    property var selectedDates: []

    Rectangle {

    }

    function indexOf(list, item) {
        var ret = -1
        for(var i = 0; i < list.length && ret === -1 ; i++) {
            ret = list[i].toString() === item.toString() ? i : ret
        }

        return ret;
    }

    DayOfWeekRow {
        locale:  multiCal.locale
        Layout.fillWidth: true
        delegate: Text {
            text: narrowName
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            required property string narrowName
        }
    }
    WeekNumberColumn {
        month: multiCal.month
        year: multiCal.year
    }

    MonthGrid {
        id: multiCal
        Layout.fillWidth: true

        locale: Qt.locale("fr_FR")

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
                    onClicked: {
                        if(!parent.isSelected)
                            selectedDates.push(model.date)
                        else
                            selectedDates.splice(indexOf(selectedDates, model.date), 1)

                        root.selectedDatesChanged()
                    }

                    anchors.fill: parent
                }
            }


            text: model.day
            color: isCurrentMonth ? "black" : "grey"
            fontSizeMode: Text.Fit
            horizontalAlignment: Qt.AlignHCenter
        }
    }
}
