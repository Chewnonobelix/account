import QtQuick 2.11
import QtQuick.Controls 1.4
import QtQuick.Window 2.12
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
    signal removeAccount (string name)
    id: mainWindow

    property int maximizedWidth: Screen.width
    Component.onCompleted: {
        showMaximized()
    }
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

    menuBar: MenuBar {
        font.family: pageStyle.core.name
        font.pixelSize: pageStyle.core.size

        objectName: "menuBar"
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Consolidate")
                objectName: "xmlMenu"
//                checkable: true
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size

                signal s_xml()
                onClicked: {
                    s_xml()
                }

                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }
            }

            MenuItem {
                text: qsTr("&Quit")
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                id: quitMenu

                Shortcut {
                    sequence: "CTRL+Q"
                    context: Qt.ApplicationShortcut
                    onActivated: quitMenu.clicked()
                }

                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }

                onClicked: {
                    mainWindow.close()
                }
            }
        }

        Menu {
            title: qsTr("Account")
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size

            MenuItem {
                text: qsTr("&New account")
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size

                onClicked: {
                    adding(true)
                }

                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }
            }
            MenuItem {
                text: qsTr("&Delete account")
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                enabled: accountSelect.model.length > 0
                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
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
        property double selectionTotal
        Label {
            id: accountTitle
            objectName: "accountTitle"
            text: qsTr("Account") + ": " + parent.accountName + " = " + parent.total + "€"
            font.pixelSize: pageStyle.title.size
            font.family: pageStyle.title.name
            padding: 10
            color: accountSelect.model.length > 0 ? parent.total > 0 ? "green" : "red" : "transparent"
        }

        Label {
            anchors.left: accountTitle.right
            anchors.leftMargin: 10
            text: qsTr("Selection total: ") + parent.selectionTotal + " €"
            font.pixelSize: pageStyle.title.size
            font.family: pageStyle.title.name
            padding: 10
            color: accountSelect.model.length > 0 ? parent.selectionTotal > 0 ? "green" : "red" : "transparent"
        }

        ComboBox {
            id: accountSelect
            objectName: "accountSelect"
            anchors.right: parent.right
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size
            enabled: accountSelect.model.length > 0
            signal s_currentTextChange(string text)

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                acceptedButtons: Qt.NoButton
            }
            Rectangle {
                gradient: pageStyle.goldButton
                anchors.fill: parent
            }

            delegate: ItemDelegate {
                width: accountSelect.width

                contentItem: Rectangle {
                    gradient: pageStyle.goldButton
                    anchors.fill: parent
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        acceptedButtons: Qt.NoButton
                    }
                    Label {
                        color: "black"
                        text: modelData
                        anchors.centerIn: parent
                        font.family: pageStyle.core.name
                        font.pixelSize: pageStyle.core.size
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
        enabled: accountSelect.model.length > 0
        anchors.top:  parent.header.bottom
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

            width: labelDelete.width * 1.2
            height: (labelDelete.height + delOk.height + 3*delOk.padding) * 1.1

            background: Rectangle {
                anchors.fill: parent
                gradient: pageStyle.backgroundGradient
                border.color: "gold"
            }


            Label {
                id: labelDelete
                property string account: accountSelect.currentText
                text: qsTr("Delete ") + account + " ?"
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                anchors.horizontalCenter: parent.width / 2
            }

            Button {
                id: delOk
                text: qsTr("Ok")
                anchors.top:labelDelete.bottom
                anchors.topMargin: padding
                font.pixelSize: pageStyle.core.size
                font.family: pageStyle.core.name


                onClicked:  {
                    mainWindow.removeAccount(labelDelete.account)
                    deleteAccount.close()
                }

                background: Rectangle {
                    id: rectEdit
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }


            }

            Button {
                id: delCancel
                anchors.left: delOk.right
                anchors.leftMargin: padding
                anchors.top:labelDelete.bottom
                anchors.topMargin: padding
                font.pixelSize: pageStyle.core.size
                font.family: pageStyle.core.name

                text: qsTr("Cancel")

                onClicked:  {
                    deleteAccount.close()
                }

                background: Rectangle {
                    id: rectEdit2

                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }
            }
        }

    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            id: listTabButton

            contentItem: Label {
                text: qsTr("List")
                horizontalAlignment: Qt.AlignHCenter
                color: tabBar.currentItem === listTabButton ? "black" : "darkseagreen"
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                acceptedButtons: Qt.NoButton
            }


            background: Rectangle {
                color: tabBar.currentItem === parent ? "darkseagreen" : "white"
            }
        }
        TabButton {
            id: graphTabButton

            contentItem: Label {
                text: qsTr("Graph")
                horizontalAlignment: Qt.AlignHCenter
                color: tabBar.currentItem === graphTabButton ? "black" : "darkseagreen"
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                acceptedButtons: Qt.NoButton
            }

            background: Rectangle {
                color: tabBar.currentItem === parent ? "darkseagreen" : "white"
            }
        }

    }
}
