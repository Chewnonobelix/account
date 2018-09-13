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

        function f(d, d2, d3) {
            console.log("d = " + d)
            console.log("d2 = " + d2)
            console.log("d3 = " + d3)
            return (d === d3)
        }

        function isSelected(sd) {
            console.log("Test = " + sd.date)
            return selectedDates.forEach(f, sd.date)
//            for(var i in selectedDates) {
//                if(sd.date === selectedDates[i]) {
//                    return true
//                }
//                else {
//                    return false
//                }
//            }
//            return false
        }

        dayDelegate: Rectangle {
            signal updateSelected()
            signal reset()
            id: styleRect


            onReset: {
                for(var i in stylesData) {
                    stylesData[i][0].color = "black"
                    stylesData[i][0].parent.color = "white"
                }

                while(selectedDates.length > 0) {
                    selectedDates.pop()

                }

            }

            onUpdateSelected: {
                console.log("Select = " + isSelected(styleData))
                if(selectedDates.indexOf(styleData.date) != -1) {
                    c_date.color = "white"
                    styleRect.color = "royalblue"
                }
                else {
                    styleRect.color = "white"
                    c_date.color = "black"
                }

            }

            Label {
                id: c_date
                anchors.centerIn: parent
                text: styleData.date.getDate()

                onTextChanged: {
                    if(cs.currentMonth != multiCal.visibleMonth) {
                        cs.currentMonth = multiCal.visibleMonth
                        parent.reset()
                    }

                    if(stylesData.indexOf(c_date) == -1) {
                        stylesData.push([c_date, styleData])
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                onClicked: {
                    if(mouse.modifiers === Qt.ShiftModifier) {
                        var index = selectedDates.indexOf(styleData.date)

                        if( index == -1) {
                            selectedDates[selectedDates.length] = styleData.date
                        }
                        else {
                            if(selectedDates.length > 1) {
                                selectedDates.splice(index,1)
                            }
                        }
                    }
                    else {
                        parent.reset()
                        selectedDates[selectedDates.length] = styleData.date
                    }
                    parent.updateSelected()
                }
            }
        }

    }


}
