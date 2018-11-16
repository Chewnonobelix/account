import QtQuick 2.11
import QtQuick.Controls 2.4

Popup {
    implicitHeight: parent.height
    implicitWidth: parent.width * .33
    x: parent.width/2 - width/2

    id: checker

    background: Rectangle {
        gradient: pageStyle.backgroundGradient
        border.color: "gold"
    }

    AccountStyle {
        id: pageStyle
    }

    ListModel {
        signal validate()

        id: checkerModel
        objectName: "checkerModel"
        function fAdd(i) {
            append(i)
        }

        function idE(i) {
            return get(i).id
        }

        function isChecked(i) {
            return get(i).isChecked
        }
    }

    ListView {
        id: listChecker
        model: checkerModel
        width: parent.width
        height: parent.height * .94
        delegate: Row {
            id: row
            CheckBox {
                id: rowChecked
                checked: isChecked

                onCheckedChanged: {
                    isChecked = checked
                }
            }
            Label {
                anchors.left: rowChecked.right
                anchors.verticalCenter: rowChecked.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: label + " " + edate + " " + value + "€"
            }
        }
    }

    Button {
        id: save
        text: qsTr("Save")
        anchors.top: listChecker.bottom
        anchors.topMargin: 10
        height: parent.height * .05

        Rectangle {
            id: saveRect
            anchors.fill: parent
            gradient: pageStyle.goldButton
        }

        onPressed: {
            saveRect.gradient = pageStyle.darkGoldButton
        }

        onReleased: {
            checkerModel.validate()
            saveRect.gradient = pageStyle.goldButton
        }
    }

    Button {
        text: qsTr("Later")
        anchors.left: save.right
        anchors.leftMargin: 10
        anchors.top: listChecker.bottom
        anchors.topMargin: 10
        height: parent.height * .05

        Rectangle {
            id: laterRect
            anchors.fill: parent
            gradient: pageStyle.goldButton
        }

        onPressed: {
            saveRect.gradient = pageStyle.darkGoldButton
        }

        onReleased: {
            saveRect.gradient = pageStyle.goldButton
            close()
        }

    }


}
