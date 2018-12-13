import QtQuick 2.11
import QtQuick.Controls 2.4

Popup {
    implicitHeight: parent.height
    implicitWidth: parent.width * .33
//    x: parent.width/2 - width/2
    anchors.centerIn: parent
    id: checker

    Label {
        text: qsTr("Entry to check")
        font.pixelSize: pageStyle.title.size
        font.family: pageStyle.title.name
        id: header
    }

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
        height: parent.height * .90
        anchors.top: header.bottom
        anchors.topMargin: 10
        flickableDirection: Flickable.AutoFlickDirection
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
                font.family: pageStyle.core.name
                font.pointSize: pageStyle.core.size
                anchors.verticalCenter: rowChecked.verticalCenter
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
        font.family: pageStyle.core.name
        font.pointSize: pageStyle.core.size

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
        font.family: pageStyle.core.name
        font.pointSize: pageStyle.core.size

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
