import QtQuick 2.11
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Calendar {
    id: multiCal

    property var selectedDates: []


    style: CalendarStyle {
        dayDelegate: Rectangle {
            function reset() {
                for(var i in selectedDates) {
                    i.color = "black"
                }

                while(selectedDates.length > 0) {
                    selectedDates.pop()
                }

            }

            function updateSelected() {
                if(selectedDates.indexOf(styleData.date.toString()) != -1) {
                    c_date.color = "red"
                }
                else {
                    c_date.color = "black"
                }

            }

            Label {
                id: c_date
                anchors.centerIn: parent
                text: styleData.date.getDate()
                color: styleData.selected ? "red" : "black"

            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if(mouse.modifiers === Qt.ShiftModifier) {
                        var index = selectedDates.indexOf(styleData.date.toString())
                        console.log(index)
                        console.log(selectedDates.length)
                        for(var i in selectedDates) {
                            console.log(i.text)
                        }

                        if( index == -1) {
                            selectedDates[styleData.date.toString()] = c_date.parent
                            console.log(c_date.parent)
                        }
                        else {
                            delete selectedDates[styleData.date.toString()]
                        }
                    }
                    else {

                        //selectedDates = [styleData.date.toString()]
                    }

                    parent.updateSelected()
                }
            }
        }
    }
}
