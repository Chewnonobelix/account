import QtQuick 2.11
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Calendar {
    id: multiCal

    property var selectedDates: []
    property var stylesData: []




    style: CalendarStyle {
        id:cs



        property int currentMonth: -1

        dayDelegate: Rectangle {
            signal updateSelected()
            signal reset()

            onReset: {
                for(var i in stylesData) {
                    stylesData[i].color = "black"
                }

                while(selectedDates.length > 0) {
                    selectedDates.pop()

                }

            }

            onUpdateSelected: {

                if(selectedDates.indexOf(styleData.index) != -1) {
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

                onTextChanged: {
                    if(cs.currentMonth != multiCal.visibleMonth) {
                        cs.currentMonth = multiCal.visibleMonth
                        parent.reset()
                    }

                    if(stylesData.indexOf(c_date) == -1) {
                        stylesData.push(c_date)
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if(mouse.modifiers === Qt.ShiftModifier) {
                        var index = selectedDates.indexOf(styleData.index)


                        if( index == -1) {
                            selectedDates[selectedDates.length] = styleData.index
                        }
                        else {
                            if(selectedDates.length > 1) {
                                delete selectedDates[index]
                            }
                        }
                    }
                    else {

                        parent.reset()
                        selectedDates[selectedDates.length] = styleData.index

                    }

                    parent.updateSelected()
                }
            }
        }

    }


}
