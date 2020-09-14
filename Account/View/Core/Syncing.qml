import QtQuick 2.15
import QtQuick.Window 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.15
import "../Style"

Window {
    objectName: "syncingpop"
    id: root

    Connections {
        target: _settings

        function onVisibleChanged(visible) {
            root.visible = visible
        }

        function onBackupChanged(back) {
            root.backup = back
        }
    }

    property bool backup: true
    flags: Qt.SplashScreen
    modality: Qt.ApplicationModal
    AccountBackground {
        anchors.fill: parent
    }

    ColumnLayout {
        anchors.fill: parent

        BusyIndicator {
            id: busy
            running: root.visible
            Layout.fillWidth: true
        }

        AccountLabel {
            Layout.fillWidth: true
            text: root.backup ? qsTr("Backup") : qsTr("Restoring") + " ..."
        }
    }
}
