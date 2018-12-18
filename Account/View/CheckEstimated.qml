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
        anchors.bottom: parent.bottom
        height: parent.height * .05
        font.family: pageStyle.core.name
        font.pointSize: pageStyle.core.size

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            acceptedButtons: Qt.NoButton
        }

        Rectangle {
            id: saveRect
            anchors.fill: parent
            gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
        }


        onClicked:  {
            checkerModel.validate()
        }
    }

    Button {
        text: qsTr("Later")
        anchors.left: save.right
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        height: parent.height * .05
        font.family: pageStyle.core.name
        font.pointSize: pageStyle.core.size

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            acceptedButtons: Qt.NoButton
        }

        Rectangle {
            id: laterRect
            anchors.fill: parent
            gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
        }

        onClicked: {
            close()
        }

    }


}
