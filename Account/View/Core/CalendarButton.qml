import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.5
import "../Style"

Button {
    id: button
    text: ""
    font.family: AccountStyle.core.name
    font.pixelSize: AccountStyle.core.size

    background: Rectangle {
        anchors.fill: parent
        gradient: pressed ? AccountStyle.goldButton : AccountStyle.darkGoldButton
    }

    property bool isOpen: pop.opened
    
    function extern(d) {
        dateAdding.selectedDate = d
    }

    onClicked: {
        pop.open()
    }
    
    Popup {
        id: pop
        background: Rectangle {
            color: "transparent"
        }
        
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
                background: Rectangle {
                    height: 250
                    width: 250
                    gradient: AccountStyle.backgroundGradient
                    border.color: "gold"
                }

                dayDelegate: Rectangle {
                    color: "transparent"
                    Label {
                        id: dLabel
                        text: styleData.date.getDate()
                        font.family: AccountStyle.core.name
                        font.pixelSize: AccountStyle.core.size
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

                    Label {
                        id:monthLabel
                        height: parent.height
                        color:"black"
                        anchors.centerIn: parent
                        text: Qt.locale().monthName(dateAdding.visibleMonth, Locale.ShortFormat) + " " + dateAdding.visibleYear
                        font.family: AccountStyle.title.name
                        font.pixelSize: height * 0.8
                    }

                    Button {
                        id: nextMonth
                        anchors.right: parent.right
                        width: dateAdding.width/14
                        height: parent.height
                        text: ">"
                        font.family: AccountStyle.core.name
                        font.pixelSize: AccountStyle.core.size

                        background: Rectangle {
                            gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
                            border.color: "darkgoldenrod"
                            anchors.fill: parent
                        }

                        MouseArea {
                            z: -1
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            acceptedButtons: Qt.NoButton
                        }

                        onClicked: {
                            dateAdding.showNextMonth()
                        }
                    }

                    Button {
                        id: prevMonth
                        anchors.left: parent.left
                        width: dateAdding.width/14
                        height: parent.height
                        font.family: AccountStyle.core.name
                        font.pixelSize: AccountStyle.core.size

                        text: "<"

                        background: Rectangle {
                            gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
                            border.color: "darkgoldenrod"
                        }

                        MouseArea {
                            z: -1
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            acceptedButtons: Qt.NoButton
                        }


                        onClicked: {
                            dateAdding.showPreviousMonth()
                        }
                    }
                }

                dayOfWeekDelegate: Rectangle {
                    height: dateAdding.height/16
                    gradient: AccountStyle.goldHeader
                    Label {
                        anchors.centerIn: parent
                        text: Qt.locale().dayName(styleData.dayOfWeek, Locale.ShortFormat)
                        font.family: AccountStyle.title.name
                    }
                }

            }
        }
    }

}
