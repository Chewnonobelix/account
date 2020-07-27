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
        background: AccountBackground{}

        GridLayout {
            anchors.fill: parent
            AccountLabel {
                text: qsTr("Session name")
                Layout.row: 0
                Layout.column: 0

                font.family: AccountStyle.title.name
                font.pixelSize: AccountStyle.title.size
            }

            AccountTextInput{
                id: commonTitle
                objectName: "commonTitle"
                Layout.columnSpan: 3
                Layout.row: 0
                Layout.column: 1
                Layout.fillWidth: true
                valid: text.length > 0
                
                ToolTip.text: qsTr("Fill a session name")
            }

            AccountButton {
                objectName: "commonAddOk"
                text: qsTr("Ok")
                Layout.column: 2
                Layout.row: 1

                onClicked: {
                    popAddCommon.close()
                    popAddCommon.s_accepted(commonTitle.text)
                }
            }
            
            AccountButton {
                text: qsTr("Cancel")
                Layout.column: 3
                Layout.row: 1

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

                delegate: AccountLabel {
                    text: modelData.title
                    height: listCommon.height * 0.07
                    width: listCommon.width

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
                AccountButton {
                    width: parent.width * .49
                    height: parent.height
                    text: qsTr("Add common expanse")
                    onClicked: popAddCommon.open()
                    
                    ToolTip.text: qsTr("Add a new common expanse")
                }
                
                AccountButton {
                    id: removeCommon
                    objectName: "removeCommon"
                    width: parent.width * .49
                    height: parent.height
                    text: qsTr("Remove common expanse")

                    signal s_remove(int id)
                    onClicked: s_remove(listCommon.currentModel.id)
                    
                    ToolTip.text: listCommon.currentModel ? qsTr("Remove") + " " + listCommon.currentModel.title : ""
                    ToolTip.visible:  hovered && listCommon.currentModel
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
