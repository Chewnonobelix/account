import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "../Style"

Popup {
    id: root

    background: AccountBackground {
    }

    topPadding: height * 0.02
    bottomPadding: height * 0.02
    leftPadding: width * 0.02
    rightPadding: width * 0.02

    onClosed: {
//        password.clear()
        profile.clear()
    }

    GridLayout {
        columnSpacing: root.width * .02
        rowSpacing: root.height * .02
        anchors.fill: parent
        columns: 3
        AccountHeader {
            text: qsTr("Profile name")
            Layout.row: 0
            Layout.column: 0
            Layout.preferredHeight: root.height * 0.34
            Layout.preferredWidth: root.width
            Layout.columnSpan: 3
        }

        //        Label {
        //            text: qsTr("Password")
        //            Layout.columnSpan: 2
        //            Layout.row: 0
        //            Layout.column: 1
        //            Layout.preferredHeight: root.height * 0.34
        //            Layout.preferredWidth: root.width * 0.47

        //            font.family: AccountStyle.title.name
        //            font.pixelSize: AccountStyle.title.size
        //            fontSizeMode: Text.Fit
        //            horizontalAlignment: Qt.AlignHCenter
        //            verticalAlignment: Qt.AlignVCenter
        //            background: Rectangle {
        //                gradient: AccountStyle.goldHeader
        //            }
        //        }

        AccountTextInput {
            id: profile
            objectName: "profileName"
            Layout.row: 1
            Layout.column: 0
            Layout.preferredHeight: root.height * 0.34
            Layout.preferredWidth: root.width
            Layout.columnSpan: 3
        }
        //        TextField {
        //            id: password
        //            objectName: "password"
        //            Layout.row: 1
        //            Layout.column: 1
        //            Layout.columnSpan: 2
        //            Layout.preferredHeight: root.height * 0.34
        //            Layout.preferredWidth: root.width * 0.47

        //            echoMode: TextInput.Password
        //            passwordMaskDelay: 1000
        //        }

        AccountButton {
            objectName: "okProfile"
            text: qsTr("Ok")
            Layout.row: 2
            Layout.column: 1
            Layout.preferredHeight: root.height * 0.22
            Layout.preferredWidth: root.width * 0.23

            onClicked: {
                _main.addProfile(profile.text)
                root.close()
            }
        }

        AccountButton {
            text: qsTr("Cancel")
            Layout.row: 2
            Layout.column: 2
            Layout.preferredHeight: root.height * 0.22
            Layout.preferredWidth: root.width * 0.22

            onClicked: {
                close()
            }
        }
    }
}
