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

    Connections {
        target: _transfert

        function onOpenChanged() {
            open()
        }

        function onAccountListChanged(list) {
            addAccount(list)
        }
    }

    background: AccountBackground {
    }

    GridLayout {
        id: coreLayout
        anchors.fill: parent
        columnSpacing: transfertView.width * 0.01
        rowSpacing: transfertView.height * 0.01

        C.CalendarButton {
            id: dateField
            objectName: "dateField"

            Layout.preferredHeight: transfertView.height * .23
            Layout.preferredWidth: transfertView.width * .23
        }


        AccountLabel {
            id:fromLabel
            text: qsTr("From")
            font.family: AccountStyle.title.name
            font.pixelSize: AccountStyle.title.size
            Layout.column: 0
            Layout.row: 1
            Layout.preferredHeight: transfertView.height * .23
            Layout.preferredWidth: transfertView.width * .23
        }

        AccountLabel {
            id:toLabel
            text: qsTr("to")
            font.family: AccountStyle.title.name
            font.pixelSize: AccountStyle.title.size
            Layout.column: 1
            Layout.row: 1
            Layout.preferredHeight: transfertView.height * .23
            Layout.preferredWidth: transfertView.width * .23
        }

        AccountLabel {
            id:valueLabel
            text: qsTr("Value")
            font.family: AccountStyle.title.name
            font.pixelSize: AccountStyle.title.size
            Layout.column: 2
            Layout.row: 1
            Layout.preferredHeight: transfertView.height * .23
            Layout.preferredWidth: transfertView.width * .23
        }

        AccountLabel {
            id:infoLabel
            text: qsTr("Info")
            font.family: AccountStyle.title.name
            font.pixelSize: AccountStyle.title.size
            Layout.column: 3
            Layout.row: 1
            Layout.preferredHeight: transfertView.height * .23
            Layout.preferredWidth: transfertView.width * .23
        }


        AccountComboBox {
            id: fromCombo
            objectName: "fromCombo"
            Layout.column: 0
            Layout.row: 2
            Layout.preferredHeight: transfertView.height * .23
            Layout.preferredWidth: transfertView.width * .23

            onCurrentIndexChanged: {
                if(currentIndex === 0) {
                    toCombo.currentIndex = 1
                } else if(currentIndex === toCombo.currentIndex) {
                    toCombo.currentIndex = 0
                }
            }
        }

        AccountComboBox {
            id: toCombo
            objectName: "toCombo"
            Layout.column: 1
            Layout.row: 2
            Layout.preferredHeight: transfertView.height * .23
            Layout.preferredWidth: transfertView.width * .23

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
            Layout.column: 2
            Layout.row: 2
            Layout.preferredHeight: transfertView.height * .23
            Layout.preferredWidth: transfertView.width * .23

        }

        AccountTextInput {
            id: fieldInfo
            objectName: "fieldInfo"
            //            width: layoutButton.width
            Layout.fillWidth: true
            ToolTip.text: qsTr("Please complete the entry's title")
            ToolTip.visible: background.border.color === "#ff0000"
            Layout.column: 3
            Layout.row: 2
            Layout.preferredHeight: transfertView.height * .23
            Layout.preferredWidth: transfertView.width * .23
        }

        AccountButton {
            text: qsTr("Ok")
            enabled: toCombo.currentText !== ""
            Layout.column: 2
            Layout.row:3
            Layout.preferredHeight: transfertView.height * .23
            Layout.preferredWidth: transfertView.width * .23

            onClicked: {
                if(fieldInfo.text.length !== 0) {
                    fieldInfo.background.border.color = "transparent"
                    _transfert.accept()
                    transfertView.close()
                } else {
                    fieldInfo.background.border.color = "red"
                }

            }
        }

        AccountButton {
            text: qsTr("Cancel")
            Layout.column: 3
            Layout.row: 3
            Layout.preferredHeight: transfertView.height * .23
            Layout.preferredWidth: transfertView.width * .23

            onClicked: transfertView.close()
        }
    }
}
