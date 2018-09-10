import QtQuick 2.11
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Calendar {
    id: multiCal

    style: CalendarStyle {
        dayDelegate: Rectangle {
            Label {
                id: c_date
                anchors.centerIn: parent
                text: styleData.date.getDate()
                color: styleData.selected ? "red" : "black"

            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("Clicked")
                    console.log(c_date.text)
                    selectedDate = styleData.date
                }
            }
        }
    }
}
