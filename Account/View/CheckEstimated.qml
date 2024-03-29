import QtQuick 2.11
import QtQuick.Controls 2.4

Popup {
    implicitHeight: parent.height
    implicitWidth: parent.width * .33

    anchors.centerIn: parent
    id: checker

    Label {
        text: qsTr("Entry to check")
        font.pixelSize: pageStyle.title.size
        font.family: pageStyle.title.name
        fontSizeMode: Text.Fit
        id: header
        anchors.left: parent.left
        anchors.right: parent.right
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

    ScrollView {
        id: listChecker
        width: parent.width
        height: parent.height * .90
        anchors.top: header.bottom
        anchors.topMargin: 10
        //        flickableDirection: Flickable.AutoFlickDirection
        //        vertical.policy: Qt.ScrollBarAsNeeded

        clip: true
        Column {
            clip: true
            Repeater {
                model: checkerModel
                clip: true
                delegate: Row {
                    id: row
                    clip: true
                    CheckBox {
                        id: rowChecked
                        checked: isChecked
                        font.family: pageStyle.core.name
                        font.pointSize: pageStyle.core.size

                        MouseArea {
                            z: -1
                            acceptedButtons: Qt.NoButton
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                        }

                        onCheckedChanged: {
                            isChecked = checked
                        }
                    }


                    Label {
                        font.family: pageStyle.core.name
                        font.pointSize: pageStyle.core.size
                        anchors.verticalCenter: rowChecked.verticalCenter
                        text: label + " " + edate + " " + value + "€"

                        MouseArea {
                            acceptedButtons: Qt.LeftButton
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true
                            onClicked: rowChecked.checked = !rowChecked.checked
                            propagateComposedEvents: true

                            ToolTip.text: qsTr("Select transaction for validation")
                            ToolTip.delay: 500
                            ToolTip.visible: containsMouse
                        }

                    }
                }
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

        ToolTip.text: qsTr("Keep validation for later")
        ToolTip.visible: hovered
        ToolTip.delay: 500

        onClicked: {
            close()
        }
    }
}
