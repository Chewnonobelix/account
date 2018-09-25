import QtQuick 2.11
import QtQuick.Controls 1.4

import QtQuick.Controls 2.2
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    visible: true

    width: 600
    height: 400

    signal adding()
    signal remove(int index)
    signal edit(int index)
    signal accountChange(int index)

    id: mainWindow

    AccountStyle {
        id: pageStyle
    }

    menuBar: MenuBar {

        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&New account")
                background: Rectangle {
                    gradient: pageStyle.goldButton
                }
            }
            MenuItem {
                text: qsTr("&Quit")
                background: Rectangle {
                    gradient: pageStyle.goldButton
                }
            }
        }

        background: Rectangle {
            gradient: pageStyle.goldHeader
        }
    }

    background: Rectangle {
        id: backRect
        anchors.fill: parent
        gradient: pageStyle.backgroundGradient
    }

    header:Rectangle {
        height: 50
        color: "transparent"
        Label {
            objectName: "accountTitle"
            text: qsTr("Account")
            font.pixelSize: Qt.application.font.pixelSize * 2
            padding: 10
        }

        ComboBox {
            id: accountSelect
            objectName: "accountSelect"
            anchors.right: parent.right

            signal s_currentTextChange(string text)

            Rectangle {
                gradient: pageStyle.goldButton
                anchors.fill: parent
            }

            delegate: ItemDelegate {
                width: accountSelect.width

                contentItem: Rectangle {
                    gradient: pageStyle.goldButton
                    anchors.fill: parent
                    Label {
                        color: "black"
                        text: modelData
                        anchors.centerIn: parent
                    }
                }

                highlighted: accountSelect.highlightedIndex === index
            }

            onCurrentTextChanged: {
                s_currentTextChange(currentText)
            }
        }
    }


    SwipeView {
        id: swipeView
        currentIndex: tabBar.currentIndex
        implicitWidth: parent.width
        implicitHeight: parent.height

        background: Rectangle {
            color: "transparent"


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
            text: qsTr("List")
        }
        TabButton {
            text: qsTr("Graph")
        }

    }
}
