import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Popup {
    AccountStyle {
        id: pageStyle
    }

    closePolicy: Popup.NoAutoClose

    function addAccount(accountList) {
        fromCombo.model = accountList
        toCombo.model = accountList

        if(accountList.length > 0) {
            fromCombo.currentIndex = 0
            toCombo.currentIndex = 1
        }
    }


    GridLayout {
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
        }

        Button {
            enabled: false
            background: Rectangle {
                color: "transparent"
            }
        }

        Button {
            enabled: false
            background: Rectangle {
                color: "transparent"
            }
        }

        Button {
            enabled: false
            background: Rectangle {
                color: "transparent"
            }
        }

        RowLayout {
            Button {
                text: qsTr("Ok")
            }

            Button {
                text: qsTr("Cancel")
            }
        }
    }
}
