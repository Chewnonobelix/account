import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../Style"

Item {
    id: root

    property var currentModel: null


    onCurrentModelChanged: {
        if(currentModel) {
            begin.extern(currentModel.begin ? currentModel.begin : new Date())
            end.extern(currentModel.end ? currentModel.end : new Date())
            syncLabel.lastSync = currentModel.lastSync

            profileList.model = _db.selectProfile()
        }
    }

    Component.onCompleted: {
        begin.extern(new Date())
        end.extern(new Date())
    }

    GridLayout {
        anchors.fill: parent
        columnSpacing: root.width * 0.02
        rowSpacing: root.height * 0.02
        anchors.leftMargin: root.width * 0.02
        anchors.rightMargin: root.width * 0.02
        anchors.topMargin: root.height * 0.02
        anchors.bottomMargin: root.height * 0.02

        AccountLabel {
            Layout.row: 0
            Layout.column: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: root.height * 0.10
            Layout.preferredWidth: root.width * 0.23

            text: qsTr("Begin date")
        }

        CalendarButton {
            Layout.row: 0
            Layout.column: 1
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: root.height * 0.10
            Layout.preferredWidth: root.width * 0.22

            id: begin

            onTextChanged: {
                if(currentModel)
                    _sync.onBeginChanged(currentModel.id, text)
            }
        }

        AccountLabel {
            Layout.row: 0
            Layout.column: 2
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: root.height * 0.10
            Layout.preferredWidth: root.width * 0.23

            text: qsTr("End date")
        }

        CalendarButton {
            Layout.row: 0
            Layout.column: 3
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: root.height * 0.10
            Layout.preferredWidth: root.width * 0.22

            id: end

            onTextChanged: {
                if(currentModel)
                    _sync.onEndChanged(currentModel.id, text)
            }
        }

        AccountLabel{
            id: syncLabel
            Layout.row: 1
            Layout.column: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 4
            Layout.preferredHeight: root.height * 0.10
            Layout.preferredWidth: root.width * 0.96

            property date lastSync: new Date()
            text: qsTr("Last syncronization") + ": " + Qt.formatDateTime(lastSync, "hh:mm:ss dd-MM-yyyy")
        }

        AccountHeader {
            Layout.row: 2
            Layout.column: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 4
            Layout.preferredHeight: root.height * 0.10
            Layout.preferredWidth: root.width * 0.96
            text: qsTr("Features to syncronize")
        }

        ScrollView {
            id: toSync
            Layout.row: 3
            Layout.column: 0
            Layout.rowSpan: 1
            Layout.columnSpan: 4
            Layout.preferredHeight: root.height * 0.60
            Layout.preferredWidth: root.width * 0.96

            ListView {
                id: profileList
                width: toSync.width * 0.2
                height: toSync.height * 2

                anchors.left: toSync.left
                anchors.top: toSync.top
                delegate: AccountLabel {
                    width: profileList.width
                    height: profileList.height * 0.5

                    text: modelData

                    background: Rectangle {
                        gradient: profileList.currentIndex === index ? AccountStyle.calSelect : AccountStyle.unselectView
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: profileList.currentIndex = index
                    }
                }
            }

            ListView {
                id: accountList
            }

        }
    }
}
