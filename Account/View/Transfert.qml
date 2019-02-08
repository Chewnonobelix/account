import QtQuick 2.12
import QtQuick.Controls 2.5

Popup {
    AccountStyle {
        id: pageStyle
    }

    width: fromCombo.width + toCombo.width + spinVal.width + fieldInfo.width + 2*fromCombo.leftPadding
    height: fromCombo.height + fromLabel.height
    closePolicy: Popup.NoAutoClose

    Label {
        id:fromLabel
        text: qsTr("From")
        anchors.bottom: fromCombo.top
        anchors.left: fromCombo.left
    }

    Label {
        id:toLabel
        text: qsTr("to")
        anchors.bottom: toCombo.top
        anchors.left: toCombo.right
    }

    Label {
        id:valueLabel
        text: qsTr("Value")
        anchors.bottom: spinVal.top
        anchors.left: spinVal.right
    }

    Label {
        id:infoLabel
        text: qsTr("Info")
        anchors.bottom: fieldInfo.top
        anchors.left: fieldInfo.right
    }


    function addAccount(accountList) {
        fromCombo.model = accountList
        toCombo.model = accountList

        if(accountList.length > 0) {
            fromCombo.currentIndex = 0
            toCombo.currentIndex = 1
        }
    }

    ComboBox {
        id: fromCombo
        objectName: "fromCombo"

        anchors.left: parent.left


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

        anchors.left: fromCombo.right
    }

    DoubleSpinBox {
        id: spinVal
        objectName: "spinVal"

        anchors.left: toCombo.right
    }

    TextField {
        id: fieldInfo
        objectName: "fieldInfo"

        anchors.left: spinVal.right
    }
}
