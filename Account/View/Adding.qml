import QtQuick 2.11
import QtQml 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4

Popup {
    //    closePolicy: Popup.NoAutoClose

    id: addingPage

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
    property string v_type: type.model.get(type.currentIndex).type
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

            CalendarButton {
                id: dateLabel
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

                ToolTip.text: addingPage.newAccount ? qsTr("Please enter an account name") : qsTr("Please complete the entry's title")
                ToolTip.visible: !valid || ttVisible
                ToolTip.delay: 100

                property bool valid: text.length !== 0
                property bool ttVisible: false
            }

            DoubleSpinBox {
                id: spinbox
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size

                ToolTip.text: addingPage.newAccount ? qsTr("Specify account opening value") : qsTr("Specify transaction's value")
                ToolTip.visible: hovered
                ToolTip.delay: 500
            }

            ComboBox {
                id: type
                objectName: "type"
                textRole: "name"
                model: ListModel {
                    id: typeModel
                    ListElement {name: qsTr("Income"); type: "income"}
                    ListElement {name: qsTr("Outcome"); type: "outcome"}
                }

                enabled: !addingid.newAccount

                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size

                ToolTip.text: qsTr("Specify income or outcome")
                ToolTip.visible: hovered
                ToolTip.delay: 500

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
                            text: model.name
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

                ToolTip.text:  addingPage.newAccount ? qsTr("Please enter an account name") : qsTr("Please complete the entry's title")
                ToolTip.visible: ttVisible

                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                    id: saveRect

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        acceptedButtons: Qt.NoButton
                    }
                }

                property bool ttVisible: false
                onClicked: {

                    if(valueLabel.valid) {
                        addingid.accept()
                        reset()
                        close()
                    }
                    else {
                        ttVisible = true
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

