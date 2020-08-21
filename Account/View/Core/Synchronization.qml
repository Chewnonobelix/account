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
        }

        AccountTextInput {
            objectName: "hostname"
        }

        AccountCheckBox {
            text: qsTr("Enable syncronization")
            objectName: "enableSync"
        }

        ListView {
            property var currentModel
            id: syncProfiles
            objectName: "syncProfiles"
        }

        CalendarButton {
            objectName: "begin"
        }

        CalendarButton {
            objectName: "end"
        }

        AccountLabel {
            text: qsTr("Last sync") + " " + Qt.formatDate(syncProfiles.currentModel.lastSync, "dd-MM-yyyy")
        }
    }
}
