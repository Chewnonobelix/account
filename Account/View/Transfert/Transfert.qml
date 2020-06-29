import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import "../Style"
import "../Core" as C
import "../Functionnal"
//import Account 1.0

Popup {
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


    background: AccountBackground {
    }

    C.CalendarButton {
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
        AccountLabel {
            id:fromLabel
            text: qsTr("From")
            font.family: AccountStyle.title.name
            font.pixelSize: AccountStyle.title.size
        }

        AccountLabel {
            id:toLabel
            text: qsTr("to")
            font.family: AccountStyle.title.name
            font.pixelSize: AccountStyle.title.size
        }

        AccountLabel {
            id:valueLabel
            text: qsTr("Value")
            font.family: AccountStyle.title.name
            font.pixelSize: AccountStyle.title.size
        }

        AccountLabel {
            id:infoLabel
            text: qsTr("Info")
            font.family: AccountStyle.title.name
            font.pixelSize: AccountStyle.title.size
        }


        AccountComboBox {
            id: fromCombo
            objectName: "fromCombo"

            onCurrentIndexChanged: {
                if(currentIndex === 0) {
                    toCombo.currentIndex = 1
                } else if(currentIndex === toCombo.currentIndex) {
                    toCombo.currentIndex = 0
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
        }

        AccountSpinbox {
            id: spinVal
            objectName: "spinVal"
        }

        AccountTextInput {
            id: fieldInfo
            objectName: "fieldInfo"
            width: layoutButton.width

            ToolTip.text: qsTr("Please complete the entry's title")
            ToolTip.visible: background.border.color === "#ff0000"
        }

    }

    RowLayout {
        id: layoutButton
        anchors.top: coreLayout.bottom
        anchors.right: coreLayout.right
        AccountButton {
            text: qsTr("Ok")

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

        AccountButton {
            text: qsTr("Cancel")

            onClicked: transfertView.close()
        }
    }
}
