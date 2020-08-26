import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15

import "../Style"

Item {
    id: root

    GridLayout {
        anchors.fill: parent

        AccountLabel {
            text: qsTr("Hostname")
            Layout.column: 0
            Layout.row: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: root.height * 0.1
            Layout.preferredWidth: root.width * 0.3

        }


        AccountTextInput {
            objectName: "hostname"
            Layout.column: 1
            Layout.row: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: root.height * 0.1
            Layout.preferredWidth: root.width * 0.3
        }

        AccountCheckBox {
            text: qsTr("Syncronization server")
            objectName: "enableSync"
            Layout.column: 2
            Layout.row: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: root.height * 0.1
            Layout.preferredWidth: root.width * 0.3
        }

        ListView {
            property var currentModel: null
            id: syncProfiles
            objectName: "syncProfiles"
            Layout.column: 0
            Layout.row: 1
            Layout.rowSpan: 2
            Layout.columnSpan: 1
            Layout.preferredHeight: root.height * 0.3
            Layout.preferredWidth: root.width * 0.3

            onCurrentIndexChanged: currentIndex > -1 ? model[currentIndex] : null
            header: AccountHeader {
                text: qsTr("Device name")
                height: syncProfiles.height * .4
                width: syncProfiles.width
            }

            delegate: Rectangle {
                height: syncProfiles.height * .2
                width: syncProfiles.width
                AccountLabel {
                    anchors.fill: parent
                    text: modelData.remoteName
                }
            }
        }

        //        CalendarButton {
        //            objectName: "begin"
        //            Layout.column: 1
        //            Layout.row: 1
        //            Layout.rowSpan: 1
        //            Layout.columnSpan: 1
        //            Layout.preferredHeight: root.height * 0.1
        //            Layout.preferredWidth: root.width * 0.1
        //            visible: syncProfiles.currentModel
        //        }

        //        CalendarButton {
        //            objectName: "end"
        //            Layout.column: 2
        //            Layout.row: 1
        //            Layout.rowSpan: 1
        //            Layout.columnSpan: 1
        //            Layout.preferredHeight: root.height * 0.1
        //            Layout.preferredWidth: root.width * 0.1
        //            visible: syncProfiles.currentModel
        //        }

        //        AccountLabel {
        //            text: qsTr("Last sync") + " " + Qt.formatDate(syncProfiles.currentModel.lastSync, "dd-MM-yyyy")
        //            Layout.column: 1
        //            Layout.row: 2
        //            Layout.rowSpan: 1
        //            Layout.columnSpan: 2
        //            Layout.preferredHeight: root.height * 0.1
        //            Layout.preferredWidth: root.width * 0.1
        //            visible: syncProfiles.currentModel
        //        }
    }
}
