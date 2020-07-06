import QtQuick 2.0
import QtQuick.Controls 2.15
AccountButton {

//    contentItem: AccountLabel {
//        horizontalAlignment: parent.icon.source === "" ? Qt.AlignHCenter : Qt.AlignLeft
//        text: parent.text
//    }

    display: AbstractButton.TextBesideIcon
    background: AccountBackground {
        invisible: true
    }
}

