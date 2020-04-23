import QtQuick 2.15
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.14

import "../Style"

Popup {
    objectName: "syncingpop"
    onOpened: busy.running = true
    onClosed: busy.running = false
    BusyIndicator {
        id: busy
        running: false
    }

    AccountLabel {
        text: qsTr("Syncing") + " ..."
    }
}
