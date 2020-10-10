import QtQuick 2.13
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.12
import "../Style"

import Account.Model 1.0
import Account 1.0

ComboBox {
    
    id: root
    
    onAccepted: {
        addCategory(editText)
    }

    property int type: Account.Outcome
    signal addCategory(string cat)
    property bool blocked: false

    model: _categoryModel

    onTypeChanged: _categoryModel.currentType = type

    textRole: "display"

    editable: currentText === ""

    background: Rectangle {
        gradient: AccountStyle.goldHeader
    }

    delegate: ItemDelegate {
        id: categoryDelegate

        width: root.width
        height: root.height

        text: model[root.textRole]

        background: Rectangle {
            gradient: AccountStyle.goldHeader
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.NoButton
            cursorShape: Qt.PointingHandCursor
        }
    }
}
