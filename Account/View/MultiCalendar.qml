import QtQuick 2.11
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Calendar {
    id: multiCal

    property var selectedDates: []
    property var stylesData: []

    signal s_datesChanged()
    readonly property string format: "dd-MM-yyyy"

    Component.onCompleted: {
        selectedDate = minimumDate
        visibleMonth = new Date().getMonth()
        console.log(new Date().getYear())
        visibleYear = 1900 + new Date().getYear()
    }

    style: CalendarStyle {
        id:cs
        property int currentMonth: -1

        function isSelected(sd) {
            var ret = false

            for(var i in selectedDates) {
                if(Qt.formatDate(sd.date, format) === selectedDates[i]) {
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
                    (stylesData[i][1].date.getMonth() === (visibleMonth)) ? "black" : "grey"
                    stylesData[i][0].parent.color = "white"
                }

                while(selectedDates.length > 0) {
                    selectedDates.pop()

                }


            }

            onUpdateSelected: {
                console.log(styleData.date)
                if(cs.isSelected(styleData) && (styleData.date.getMonth() === visibleMonth)) {
                    c_date.color = "white"
                    styleRect.color = "royalblue"
                }
                else if(styleData.date.getMonth() === visibleMonth){
                    styleRect.color = "white"
                    c_date.color = "black"
                    console.log("Color")
                }
                multiCal.s_datesChanged()
            }

            Label {
                id: c_date
                anchors.centerIn: parent
                text: styleData.date.getDate()
                color: styleData.date.getMonth() === (multiCal.visibleMonth) ? "black" : "grey"
                onTextChanged: {
                    /*if(cs.currentMonth != (visibleMonth + 1)) {
                        cs.currentMonth = multiCal.visibleMonth
                        parent.reset()
                    }*/

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
                            selectedDate = minimumDate
                        }
                    }
                    parent.updateSelected()

                }
            }
        }

    }


}
