import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "../Style"

Popup {
    id: root
    AccountStyle {
        id: style
    }

    background: Rectangle {
        gradient: style.backgroundGradient
    }

    GridLayout {
        columnSpacing: root.width * .02
        rowSpacing: root.height * .02
        anchors.fill: parent
        columns: 3
        Label {
            text: qsTr("Profile name")
            Layout.row: 0
            Layout.column: 0
            Layout.preferredHeight: root.height * 0.40
            Layout.preferredWidth: root.width * 0.47

            font.family: style.title.name
            font.pixelSize: style.title.size
            fontSizeMode: Text.Fit
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
        }

        Label {
            text: qsTr("Password")
            Layout.columnSpan: 2
            Layout.row: 0
            Layout.column: 1
            Layout.preferredHeight: root.height * 0.40
            Layout.preferredWidth: root.width * 0.47
            font.family: style.title.name
            font.pixelSize: style.title.size
            fontSizeMode: Text.Fit
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
        }

        TextField {
            objectName: "profileName"
            Layout.row: 1
            Layout.column: 0
            Layout.preferredHeight: root.height * 0.40
            Layout.preferredWidth: root.width * 0.47
        }

        TextField {
            objectName: "password"
            Layout.row: 1
            Layout.column: 1
            Layout.columnSpan: 2
            Layout.preferredHeight: root.height * 0.40
            Layout.preferredWidth: root.width * 0.47

            echoMode: TextInput.PasswordEchoOnEdit
            passwordMaskDelay: 2000
        }

        Button {
            text: qsTr("Ok")
            Layout.row: 2
            Layout.column: 1
            Layout.preferredHeight: root.height * 0.12
            Layout.preferredWidth: root.width * 0.23
        }

        Button {
            text: qsTr("Cancel")
            Layout.row: 2
            Layout.column: 2
            Layout.preferredHeight: root.height * 0.12
            Layout.preferredWidth: root.width * 0.22
        }
    }
}
