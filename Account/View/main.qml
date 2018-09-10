import QtQuick 2.9
import QtQuick.Controls 1.4

import QtQuick.Controls 2.2
import QtQuick.Controls 2.4

ApplicationWindow {
    visible: true

    width: 600
    height: 400

    signal adding()
    signal remove(int index)
    signal edit(int index)
    signal accountChange(int index)

    id: mainWindow

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {text: qsTr("&New account") }
            Action {text: qsTr("&Load account") }
            Action {text: qsTr("&Save") }
            Action {text: qsTr("&Quit") }
        }
    }

    header: Label {
        text: qsTr("Account")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }


    SwipeView {
        id: swipeView
        currentIndex: tabBar.currentIndex
        implicitWidth: parent.width
        implicitHeight: parent.height

        MultiCalendar {
        }

        Page1Form {
            id: table
            objectName: "table"
        }

        Page2Form {
            id: graph
        }

    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Info")
        }

        TabButton {
            text: qsTr("List")
        }
        TabButton {
            text: qsTr("Graph")
        }

    }
}
