import QtQuick 2.11
import QtQuick.Controls 1.4

import QtQuick.Controls 2.2
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    visible: true

    width: 800
    height: 640

    signal adding(bool account)
    signal remove(int index)
    signal edit(int index)
    signal accountChange(int index)

    id: mainWindow


    Component.onCompleted: showMaximized()

    Shortcut {
        sequence: "CTRL+N"
        context: Qt.ApplicationShortcut

        onActivated: {
            adding(true)
        }
    }

    AccountStyle {
        id: pageStyle
    }

    MouseArea {
        anchors.fill: parent
        id: mainarea
    }

    menuBar: MenuBar {
        font.family: pageStyle.core.name

        objectName: "menuBar"
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Xml")
                objectName: "xmlMenu"
                checkable: true
                font.family: pageStyle.core.name

                signal s_xml(bool toXml)
                onClicked: {
                    s_xml(checked)
                }

                background: Rectangle {
                    gradient: pageStyle.goldButton
                }
            }

            MenuItem {
                text: qsTr("&Quit")
                font.family: pageStyle.core.name

                background: Rectangle {
                    gradient: pageStyle.goldButton
                }
            }
        }

        Menu {
            title: qsTr("Account")
            font.family: pageStyle.core.name

            MenuItem {
                text: qsTr("&New account")
                font.family: pageStyle.core.name

                onClicked: {
                    adding(true)
                }

                background: Rectangle {
                    gradient: pageStyle.goldButton
                }
            }
            MenuItem {
                text: qsTr("&Delete account")
                font.family: pageStyle.core.name

                background: Rectangle {
                    gradient: pageStyle.goldButton
                }

                onClicked: {
                    deleteAccount.open()
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
        id: head
        objectName:  "head"
        property string accountName
        property double total

        Label {
            objectName: "accountTitle"
            text: qsTr("Account") + ": " + parent.accountName + " = " + parent.total + "€"
            font.pixelSize: Qt.application.font.pixelSize * 2
            font.family: pageStyle.title.name
            padding: 10
            color: parent.total > 0 ? "green" : "red"
        }

        ComboBox {
            id: accountSelect
            objectName: "accountSelect"
            anchors.right: parent.right
            font.family: pageStyle.core.name

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
                        font.family: pageStyle.core.name
                    }
                }

                highlighted: accountSelect.highlightedIndex === index
            }

            onCurrentTextChanged: {
                s_currentTextChange(currentText)
            }
        }
    }

    CheckEstimated {
        objectName: "cEstimated"
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

        Popup {
            id: deleteAccount
            anchors.centerIn: swipeView
            width: delOk.width + delCancel.width + 10 + 2*delOk.padding + 2*delCancel.padding
            height: delOk.height + 4*delOk.padding

            background: Rectangle {
                anchors.fill: parent
            }

            onOpened: {
                console.log(width +" " + height)
                console.log(delOk.width +" " + delOk.height)
                console.log(delOk.padding)
            }

            Label {
                id: labelDelete
                property string account: accountSelect.currentText
                text: qsTr("Delete ") + account + " ?"
            }

            Button {
                id: delOk
                text: qsTr("Ok")
                anchors.top:labelDelete.bottom
                anchors.topMargin: padding

                onClicked: {
                    console.log("ok")
                    deleteAccount.close()
                }

            }

            Button {
                id: delCancel
                anchors.left: delOk.right
                anchors.leftMargin: padding
                anchors.top:labelDelete.bottom
                anchors.topMargin: padding

                text: qsTr("Cancel")
                onClicked: {
                    deleteAccount.close()
                }
            }
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
