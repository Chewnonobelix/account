import QtQuick 2.11
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Calendar {
    id: multiCal

    property var selectedDates: []
    property var stylesData: []

    signal s_datesChanged()

    style: CalendarStyle {
        id:cs

        property int currentMonth: -1

        function isSelected(sd) {
            var ret = false

            for(var i in selectedDates) {
                if(sd.date.toString() === selectedDates[i].toString()) {
                    ret = true
                }
            }
            return ret
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
                if(cs.isSelected(styleData) && (styleData.date.getMonth() === visibleMonth)) {
                    c_date.color = "white"
                    styleRect.color = "royalblue"
                }
                else {
                    styleRect.color = "white"
                    c_date.color = "black"
                }
                multiCal.s_datesChanged()
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
                        var index = -1
                        for(var i in selectedDates) {
                            if(styleData.date.toString() === selectedDates[i].toString()) {
                                index = i
                            }

                        }
                        if( index == -1) {
                            selectedDates[selectedDates.length] = styleData.date
                        }
                        else {
                            selectedDates.splice(index,1)
                        }
                    }
                    else {
                        parent.reset()
                        console.log(selectedDate)
                        console.log(styleData.date)
                        console.log(selectedDates.length)
                        if(!styleData.selected && selectedDate.toString() !== styleData.date.toString()) {
                            selectedDates[selectedDates.length] = styleData.date
                            selectedDate = styleData.date
                        }
                        else {
                            delete selectedDate
                        }
                    }
                    parent.updateSelected()

                    console.log(styleData.selected)
                }
            }
        }

    }


}
