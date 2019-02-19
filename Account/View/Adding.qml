import QtQuick 2.11
import QtQml 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4

Popup {
    //    closePolicy: Popup.NoAutoClose

    function reset() {
        valueLabel.text = ""
        spinbox.value = 0
        dateModel.clear()
    }

    AccountStyle {
        id: pageStyle
    }


    signal accept()

    property real v_val: spinbox.realValue
    property string v_title: valueLabel.text
    property string v_type: type.currentText
    property string v_date: dateLabel.text
    property bool newAccount: false

    onClosed: {
        dateModel.clear()
        valueLabel.background.border.color = "#bdbdbd"
    }

    onOpened: {
        l_new.text = newAccount ? qsTr("Add new account"): qsTr("Add new entry")
    }

    background: Rectangle {
        gradient: pageStyle.backgroundGradient
        border.color: "darkgoldenrod"
    }


    ListModel {
        id: dateModel
    }

    function addDate(d) {
        dateModel.append({"text:": d})
    }

    ColumnLayout {
        RowLayout {
            Label {
                id: labelAdd
                text: qsTr("Adding")
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
            }

            Button {
                id: dateLabel

                text: Qt.formatDate(dateAdding.selectedDate, "dd-MM-yyyy")
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size

                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }

                onClicked: datePop.open()

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    acceptedButtons: Qt.NoButton
                }
            }

            Popup {
                id:datePop

                Calendar {
                    id: dateAdding
                    selectedDate: new Date()
                    onClicked: datePop.close()
                    height: 250
                    width: 250

                    style: CalendarStyle {
                        gridColor: "goldenrod"
                        gridVisible: true
                        background: Rectangle {
                            height: 250
                            width: 250
                            gradient: pageStyle.backgroundGradient
                            border.color: "gold"
                        }

                        dayDelegate: Rectangle {
                            color: "transparent"
                            Label {
                                id: dLabel
                                text: styleData.date.getDate()
                                font.family: pageStyle.core.name
                                font.pixelSize: pageStyle.core.size
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

                            gradient: pageStyle.goldHeader

                            Label {
                                id:monthLabel
                                height: parent.height
                                color:"black"
                                anchors.centerIn: parent
                                text: Qt.locale().monthName(dateAdding.visibleMonth, Locale.ShortFormat) + " " + dateAdding.visibleYear
                                font.family: pageStyle.title.name
                                font.pixelSize: height * 0.8
                            }

                            Button {
                                id: nextMonth
                                anchors.right: parent.right
                                width: dateAdding.width/14
                                height: parent.height
                                text: ">"
                                font.family: pageStyle.core.name
                                font.pixelSize: pageStyle.core.size

                                background: Rectangle {
                                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
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
                                font.family: pageStyle.core.name
                                font.pixelSize: pageStyle.core.size

                                text: "<"

                                background: Rectangle {
                                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
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
                            gradient: pageStyle.goldHeader
                            Label {
                                anchors.centerIn: parent
                                text: Qt.locale().dayName(styleData.dayOfWeek, Locale.ShortFormat)
                                font.family: pageStyle.title.name
//                                font.pixelSize: height * 0.55
                            }
                        }
                    }
                }
            }
        }

        RowLayout {
            Label {
                id: l_new
                text: qsTr("Add new entry")
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
            }

            TextField {
                id: valueLabel
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size

                ToolTip.text: qsTr("Please complete the entry's title")
                ToolTip.visible: background.border.color === "#ff0000"
            }

            DoubleSpinBox {
                id: spinbox
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
            }

            ComboBox {
                id: type
                objectName: "type"
                model: [qsTr("Income"), qsTr("Outcome")]
                enabled: !addingid.newAccount

                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size

                Rectangle {
                    gradient: pageStyle.goldButton
                    anchors.fill: parent
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    acceptedButtons: Qt.NoButton
                }
                delegate: ItemDelegate {
                    width: accountSelect.width

                    contentItem: Rectangle {
                        gradient: pageStyle.goldButton
                        anchors.fill: parent
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            acceptedButtons: Qt.NoButton
                        }
                        Label {
                            color: "black"
                            text: modelData
                            anchors.centerIn: parent
                            font.family: pageStyle.core.name
                            font.pixelSize: pageStyle.core.size
                        }
                    }

                    highlighted: accountSelect.highlightedIndex === index
                }
            }
        }

        RowLayout {
            Button {
                id: b_save
                text: qsTr("Save")
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size

                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                    id: saveRect

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        acceptedButtons: Qt.NoButton
                    }
                }


                onClicked: {

                    if(valueLabel.text.length !== 0) {
                        addingid.accept()
                        reset()
                        close()
                    }
                    else {
                        valueLabel.background.border.color = "red"
                    }
                }
            }

            Button {
                id:b_cancel
                text: qsTr("Cancel")
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                    id: cancelRect
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    acceptedButtons: Qt.NoButton
                }

                onClicked: {
                    reset()
                    close()
                }
            }
        }
    }
}

