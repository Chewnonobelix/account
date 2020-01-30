import QtQuick 2.0

import QtQuick.Controls 1.4
import QtQuick.Window 2.12
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.13

import "../Style"


Rectangle {
    id: root
    color: "transparent"
    objectName: "commonRect"

    Popup {
        id: popAddCommon
        objectName: "popAddCommon"
        onOpened: commonTitle.clear()
        signal s_accepted(string name)

        anchors.centerIn: parent
        background: Rectangle {
            gradient: AccountStyle.backgroundGradient
            border.color: "gold"
        }

        GridLayout {
            anchors.fill: parent
            Label {
                text: qsTr("Session name")
                Layout.row: 0
                Layout.column: 0

                font.family: AccountStyle.title.name
                font.pixelSize: AccountStyle.title.size
                fontSizeMode: Text.Fit
            }

            TextField{
                id: commonTitle
                objectName: "commonTitle"
                Layout.columnSpan: 3
                Layout.row: 0
                Layout.column: 1
                Layout.fillWidth: true
                font.family: AccountStyle.core.name
                font.pixelSize: AccountStyle.core.size

                property bool valid: text.length > 0


                background: Rectangle {
                    border.color: parent.valid ? "blue" : "red"
                }
            }

            Button {
                objectName: "commonAddOk"
                text: qsTr("Ok")
                Layout.column: 2
                Layout.row: 1

                onClicked: {
                    popAddCommon.close()
                    popAddCommon.s_accepted(commonTitle.text)
                }

                background: Rectangle {
                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.NoButton
                        cursorShape: Qt.PointingHandCursor
                    }

                    gradient: AccountStyle.goldButton
                }
            }
            Button {
                text: qsTr("Cancel")
                Layout.column: 3
                Layout.row: 1

                background: Rectangle {
                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.NoButton
                        cursorShape: Qt.PointingHandCursor
                    }

                    gradient: AccountStyle.goldButton
                }

                onClicked: popAddCommon.close()
            }
        }
    }

    Row {
        anchors.fill: parent
        spacing: width * 0.02
        Column{
            height: parent.height
            width: parent.width * 0.20
            spacing: height * .02
            ListView {
                id: listCommon
                objectName: "listCommon"
                width: parent.width
                height: parent.height * 0.93
                clip: true

                Component.onCompleted: {
                    currentModel = Qt.binding(function() {return currentIndex > -1 ? model[currentIndex] : null })
                    commonExpanse.model = Qt.binding(function() {return currentModel})
                    commonExpanse.enabled = Qt.binding(function() {return currentIndex > -1})
                    removeCommon.enabled = Qt.binding(function() {return currentIndex > -1})
                }

                property var currentModel
                Rectangle{
                    anchors.fill: parent
                    color: "transparent"
                    border.color: "gold"
                }

                highlight: Rectangle {
                    gradient: AccountStyle.calSelect
                }

                highlightMoveDuration: 0

                delegate: Label {
                    text: modelData.title
                    height: listCommon.height * 0.07
                    width: listCommon.width

                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter

                    fontSizeMode: Text.Fit
                    font.family: AccountStyle.core.name
                    font.pixelSize: AccountStyle.core.size

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: listCommon.currentIndex = listCommon.currentIndex === index ? -1 : index
                    }
                }
            }

            Row {
                width: parent.width
                height: parent.height * 0.05
                spacing: width * 0.02
                Button {
                    width: parent.width * .49
                    height: parent.height
                    text: qsTr("Add common expanse")
                    onClicked: popAddCommon.open()

                    font.family: AccountStyle.core.name
                    font.pixelSize: AccountStyle.core.size

                    background: Rectangle {
                        MouseArea{
                            acceptedButtons: Qt.NoButton
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                        }

                        gradient: AccountStyle.goldButton
                    }
                }
                Button {
                    id: removeCommon
                    objectName: "removeCommon"
                    width: parent.width * .49
                    height: parent.height
                    text: qsTr("Remove common expanse")

                    font.family: AccountStyle.core.name
                    font.pixelSize: AccountStyle.core.size

                    signal s_remove(int id)
                    onClicked: s_remove(listCommon.currentModel.id)

                    background: Rectangle {
                        MouseArea{
                            acceptedButtons: Qt.NoButton
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                        }
                        gradient: AccountStyle.goldButton
                    }
                }
            }
        }

        CommonExpanseView {
            id: commonExpanse
            objectName: "common"
            height: parent.height
            width: parent.width * .78
        }
    }
}
