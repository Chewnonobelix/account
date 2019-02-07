import QtQuick 2.12
import QtQuick.Controls 2.5

Popup {
    AccountStyle {
        id: pageStyle
    }

    width: fromCombo.width + toCombo.width + spinVal.width + fieldInfo.width + 2*fromCombo.leftPadding
    height: fromCombo.height + fromLabel.height

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

    property list accounts

    function addAccount(accountList) {
        accounts = accountList
        fromCombo.model = accountList
        toCombo.model = accountList
    }

    ComboBox {
        id: fromCombo
        objectName: "fromCombo"

        anchors.left: parent.left
        onCurrentTextChanged: {

        }

    }

    ComboBox {
        id: toCombo
        objectName: "toCombo"

        anchors.left: fromCombo.right
        onCurrentTextChanged: {

        }
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
