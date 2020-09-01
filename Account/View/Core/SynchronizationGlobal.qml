import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15

import "../Style"

Item {
    id: root

    onVisibleChanged: {
        enableSync.checked = _settings.syncServer()
    }

    function applied() {
        _settings.setSyncServer(enableSync.checked)
        _sync.openServer(enableSync.checked)
    }

    Connections {
        target: _sync
        function onConnectionListChanged(connectionsList) {
            syncProfiles.model = connectionsList
        }
    }

    GridLayout {
        anchors.fill: parent

        rowSpacing: root.height * 0.02
        columnSpacing: root.width * 0.02
        AccountLabel {
            text: qsTr("Hostname")
            Layout.column: 0
            Layout.row: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: root.height * 0.1
            Layout.preferredWidth: root.width * 0.3
            Layout.alignment: Qt.AlignTop
            Layout.topMargin: root.height * 0.02
        }


        AccountTextInput {
            objectName: "hostname"
            Layout.column: 1
            Layout.row: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: root.height * 0.1
            Layout.preferredWidth: root.width * 0.3
            Layout.alignment: Qt.AlignTop
            Layout.topMargin: root.height * 0.02
        }

        AccountCheckBox {
            text: qsTr("Syncronization server")
            id: enableSync
            objectName: "enableSync"
            Layout.column: 2
            Layout.row: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: root.height * 0.1
            Layout.preferredWidth: root.width * 0.3
            Layout.alignment: Qt.AlignTop
            Layout.topMargin: root.height * 0.02
        }

        ListView {
            visible: enableSync.checked
            property var currentModel: null
            id: syncProfiles
            objectName: "syncProfiles"
            Layout.column: 0
            Layout.row: 1
            Layout.rowSpan: 2
            Layout.columnSpan: 1
            Layout.preferredHeight: root.height * 0.9
            Layout.preferredWidth: root.width * 0.3

            onModelChanged: currentIndex = -1

            onCurrentIndexChanged: {
                currentModel = currentIndex > -1 ? model[currentIndex] : null

                if(currentModel)
                    currentSync.currentModel = currentModel.localProfil
            }

            header: AccountHeader {
                text: qsTr("Device name")
                height: syncProfiles.height * .1
                width: syncProfiles.width
            }

            delegate: Rectangle {
                height: syncProfiles.height * .2
                width: syncProfiles.width

                gradient: index === syncProfiles.currentIndex ? AccountStyle.calSelect : AccountStyle.unselectView
                readonly property string nullid: "{00000000-0000-0000-0000-000000000000}"

                AccountLabel {
                    Rectangle {

                    anchors.fill: parent
                    text: modelData.remoteName
                    color: modelData.localProfile.id === nullid ? "red" : "black"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked:  {
                        if(syncProfiles.currentIndex !== index) {
                            syncProfiles.currentIndex = index
                            syncProfiles.currentModel = modelData
                            enableSyncProf.checked = modelData.localProfile.id !== nullid
                        }
                        else {
                            syncProfiles.currentIndex = -1
                        }
                    }
                }
            }
        }

        GroupBox {
            id: groupSync
            visible: syncProfiles.currentModel && enableSync.checked
            label: AccountCheckBox {
                id: enableSyncProf
                checked: syncProfiles.currentModel ? syncProfiles.currentModel.localProfile.id != "{00000000-0000-0000-0000-000000000000}" : false
                text: syncProfiles.currentModel ? syncProfiles.currentModel.remoteName : ""

                onCheckedChanged: {
                    if(checked) syncProfiles.currentModel.addLocalProfile()
                    else syncProfiles.currentModel.removeLocalProfile();
                }
            }

            Layout.column: 1
            Layout.row: 1
            Layout.rowSpan: 2
            Layout.columnSpan: 2
            Layout.preferredHeight: root.height * 0.9
            Layout.preferredWidth: root.width * 0.7

            Syncronization {
                enabled: enableSyncProf.checked
                id: currentSync
                anchors.fill: parent
                currentModel: syncProfiles.currentModel
            }


        }
    }
}
