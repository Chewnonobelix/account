import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14

Window {
    height: 500
    width: height * 9 / 16

    flags: Qt.Popup

    onActiveChanged: if(!active) close()
    StackView {
        anchors.fill: parent
        Rectangle {
            anchors.fill: parent
            CalendarButton {

            }
        }

        EntryEdit {
            anchors.fill: parent
            Component.onCompleted: changeDirection()
        }
    }
}
