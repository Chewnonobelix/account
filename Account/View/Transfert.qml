import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Popup {
    AccountStyle {
        id: pageStyle
    }

    id: transfertView
//    closePolicy: Popup.NoAutoClose

    signal s_accept()

    width: coreLayout.width + 20
    height: coreLayout.height + dateField.height + layoutButton.height + 20


    function addAccount(accountList) {
        fromCombo.model = accountList
        toCombo.model = accountList
        spinVal.value = 0
        fieldInfo.clear()

        if(accountList.length > 0) {
            fromCombo.currentIndex = 0
            toCombo.currentIndex = 1
        }
    }


    background: Rectangle {
        border.color: "gold"
        gradient: pageStyle.backgroundGradient
    }

    CalendarButton {
        id: dateField
        objectName: "dateField"

        width: 80
        height: 25
    }

    GridLayout {
        id: coreLayout
        anchors.top: dateField.bottom
        columns: 4
        columnSpacing: 5
        Label {
            id:fromLabel
            text: qsTr("From")
        }

        Label {
            id:toLabel
            text: qsTr("to")
        }

        Label {
            id:valueLabel
            text: qsTr("Value")
        }

        Label {
            id:infoLabel
            text: qsTr("Info")
        }


        ComboBox {
            id: fromCombo
            objectName: "fromCombo"

            Rectangle {
                anchors.fill: parent
                gradient: pageStyle.goldButton
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                acceptedButtons: Qt.NoButton
            }

            onCurrentIndexChanged: {
                if(currentIndex === 0) {
                    toCombo.currentIndex = 1
                } else if(currentIndex === toCombo.currentIndex) {
                    toCombo.currentIndex = 0
                }
            }

            delegate: ItemDelegate {
                width: fromCombo.width

                contentItem: Rectangle {
                    anchors.fill: parent
                    gradient:  pageStyle.goldButton
                    Text {
                        id: delegateText
                        text: modelData
                        anchors.centerIn: parent
                    }
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        acceptedButtons: Qt.NoButton
                    }
                }
            }
        }

        ComboBox {
            id: toCombo
            objectName: "toCombo"

            Rectangle {
                anchors.fill: parent
                gradient: pageStyle.goldButton
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                acceptedButtons: Qt.NoButton
            }

            onCurrentIndexChanged: {
                if(currentIndex === 0) {
                    fromCombo.currentIndex = 1
                } else if(currentIndex === fromCombo.currentIndex) {
                    fromCombo.currentIndex = 0
                }
            }

            delegate: ItemDelegate {
                width: toCombo.width

                contentItem: Rectangle {
                    anchors.fill: parent
                    gradient: pageStyle.goldButton
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        acceptedButtons: Qt.NoButton
                    }
                    Text {
                        id: delegateText2
                        text: modelData
                        anchors.centerIn: parent
                    }
                }
            }
        }

        DoubleSpinBox {
            id: spinVal
            objectName: "spinVal"
        }

        TextField {
            id: fieldInfo
            objectName: "fieldInfo"
            width: layoutButton.width

            onWidthChanged: console.log("width", width)
            ToolTip.text: qsTr("Please complete the entry's title")
            ToolTip.visible: background.border.color === "#ff0000"
        }

        Button {
            enabled: false
            background: Rectangle {
                color: "transparent"
            }
        }

    }

    RowLayout {
        id: layoutButton
        anchors.top: coreLayout.bottom
        anchors.right: coreLayout.right
        Button {
            text: qsTr("Ok")
            background: Rectangle {
                gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
            }

            onClicked: {
                if(fieldInfo.text.length !== 0) {
                    fieldInfo.background.border.color = "transparent"
                    transfertView.s_accept()
                    transfertView.close()
                } else {
                    fieldInfo.background.border.color = "red"
                }

            }
        }

        Button {
            text: qsTr("Cancel")
            background: Rectangle {
                gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
            }

            onClicked: transfertView.close()
        }
    }
}
