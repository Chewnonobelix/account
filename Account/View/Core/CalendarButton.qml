import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.5
import "../Style"

AccountButton {
    id: button
    text: ""

    property bool isOpen: pop.opened
    
    function extern(d) {
        dateAdding.selectedDate = d
    }

    onClicked: {
        pop.open()
    }
    
    Popup {
        id: pop
        background: AccountBackground{invisible: true}
        
        Calendar {
            id: dateAdding
            selectedDate: new Date()
            onClicked: pop.close()
            height: 250
            width: 250

            onSelectedDateChanged: {
                button.text = Qt.formatDate(dateAdding.selectedDate, "dd-MM-yyyy")
            }

            style: CalendarStyle {
                gridColor: "goldenrod"
                gridVisible: true
                background: AccountBackground {
                    height: 250
                    width: 250
                }

                dayDelegate: Rectangle {
                    color: "transparent"
                    AccountLabel {
                        id: dLabel
                        text: styleData.date.getDate()
                        anchors.centerIn: parent
                        color: styleData.date.getMonth() === dateAdding.visibleMonth ? "black" : "grey"
                    }
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: styleData.date.getMonth() === dateAdding.visibleMonth ? Qt.PointingHandCursor : Qt.ArrowCursor
                        acceptedButtons: Qt.NoButton
                    }
                }

                navigationBar: Rectangle {
                    id: navBar
                    height: dateAdding.height/16

                    gradient: AccountStyle.goldHeader

                    AccountLabel {
                        id:monthLabel
                        height: parent.height
                        color:"black"
                        anchors.centerIn: parent
                        text: Qt.locale().monthName(dateAdding.visibleMonth, Locale.ShortFormat) + " " + dateAdding.visibleYear
                        font.family: AccountStyle.title.name
                        font.pixelSize: height * 0.8
                    }

                    AccountButton {
                        id: nextMonth
                        anchors.right: parent.right
                        width: dateAdding.width/14
                        height: parent.height
                        text: ">"

                        onClicked: {
                            dateAdding.showNextMonth()
                        }
                    }

                    AccountButton {
                        id: prevMonth
                        anchors.left: parent.left
                        width: dateAdding.width/14
                        height: parent.height

                        text: "<"

                        onClicked: {
                            dateAdding.showPreviousMonth()
                        }
                    }
                }

                dayOfWeekDelegate: Rectangle {
                    height: dateAdding.height/16
                    gradient: AccountStyle.goldHeader
                    AccountLabel {
                        anchors.centerIn: parent
                        text: Qt.locale().dayName(styleData.dayOfWeek, Locale.ShortFormat)
                        font.family: AccountStyle.title.name
                    }
                }

            }
        }
    }

}
