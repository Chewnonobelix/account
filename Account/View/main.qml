import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 2.4

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Tabs")

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {text: qsTr("&New account") }
            Action {text: qsTr("&Load account") }
            Action {text: qsTr("&Save") }
            Action {text: qsTr("&Quit") }
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page1Form {
        }

        Page2Form {
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("List")
        }
        TabButton {
            text: qsTr("Graph")
        }
    }
}
