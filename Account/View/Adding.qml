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
        dateCombo.currentIndex = 0
    }

    AccountStyle {
        id: pageStyle
    }


    signal accept()

    property real v_val: spinbox.realValue
    property string v_title: valueLabel.text
    property string v_type: type.currentText
    property string v_date: dateCombo.currentText
    property bool newAccount: false

    onClosed: {
        dateModel.clear()
        valueLabel.background.border.color = "#bdbdbd"
    }

    onOpened: {
        dateCombo.currentIndex = 0
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
        Label {
            id: labelAdd
            text: qsTr("Adding " + v_date)
            font.family: pageStyle.title.name
            font.pixelSize: pageStyle.title.size
        }


        ComboBox {
            id: dateCombo
            model: dateModel
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
                model: ["Income", "Outcome"]
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

