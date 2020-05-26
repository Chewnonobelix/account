import QtQuick 2.11
import Qt.labs.platform 1.1 as P

import QtQuick.Controls 1.4
import QtQuick.Window 2.12
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.13

import "../Style"
import "../Transfert" as T
import "../CommonExpanse"
import "../Budget"
import "../Frequency"
import "../MultiProfile"
import "../Graph"
import "../Functionnal"

ApplicationWindow {
    visible: true
    
    width: 800
    height: 640
    
    signal adding(bool account)
    signal remove(int index)
    signal edit(int index)
    signal accountChange(int index)
    signal removeAccount(string name)
    signal openTransfert
    signal s_closing()
    signal s_openFrequencyManager()
    signal s_openSetting()

    id: mainWindow
    
    onClosing: s_closing()

    Settings {
        objectName: "settings"
    }

    onVisibilityChanged: {
        if(visibility === Window.Minimized)
            hide()
    }

    QuickAdding {
        id: quick
        objectName: "quick"
        x: Screen.width - width
        y: Screen.desktopAvailableHeight - height

        signal s_opening()

        onVisibleChanged: if(visible) s_opening()
    }

    P.SystemTrayIcon {
        id: tray
        visible: true
        icon.source: "qrc:/Style/tray.png"

        function reopen() {
            if(mainWindow.visibility === Window.Hidden) {
                mainWindow.requestActivate()
                mainWindow.show()
            }
            else {
                mainWindow.hide()
            }
        }

        onActivated: {
            if(reason === P.SystemTrayIcon.DoubleClick) {
                reopen()
            }

            if(reason === P.SystemTrayIcon.Context) {
                show()
            }
        }

        menu: P.Menu {
            P.MenuItem {
                text: qsTr("Quick add")
                onTriggered: quick.show()
            }

            P.MenuItem {
                text: mainWindow.visibility === Window.Hidden ? qsTr("Show normal") : qsTr("Hide")
                onTriggered: tray.reopen()
            }

            P.MenuItem {
                text: qsTr("Quit")
                onTriggered: mainWindow.close()
            }
        }
    }

    property int maximizedWidth: Screen.width
    Component.onCompleted: {
        showMaximized()
    }
    
    T.Transfert {
        id: tranfert
        objectName: "transfert"
        
        anchors.centerIn: parent
        onOpened: {
            addAccount(accountSelect.model)
        }
    }
    
    Shortcut {
        sequence: "F"
        onActivated: openTransfert()
    }

    NewProfile {
        id: popProfile
        objectName: "popProfile"
        width: 300
        height: 100
        anchors.centerIn: parent
    }
    
    menuBar: MenuBar {
        font.family: AccountStyle.core.name
        font.pixelSize: AccountStyle.core.size
        
        objectName: "menuBar"
        Menu {
            title: qsTr("&File")

            width: 210
            delegate: MenuItem {
                background: Rectangle {
                    gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
                }
            }

            MenuItem {
                text: qsTr("Settings")
                onTriggered: s_openSetting()
                background: Rectangle {
                    gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
                }
            }

            Menu {
                id: profileMenu
                objectName: "profileMenu"
                width: 210
                title: qsTr("Profile")
                signal s_profile(string name)
                font.family: AccountStyle.core.name
                font.pixelSize: AccountStyle.core.size

                Repeater {
                    id: profileRepeater
                    objectName: "profileRepeater"
                    property string current
                    MenuItem {
                        text: modelData
                        checked: modelData === profileRepeater.current
                        autoExclusive: true
                        checkable: true
                        onTriggered: {
                            profileRepeater.current = text
                            profileMenu.s_profile(text)
                        }
                        font.family: AccountStyle.core.name
                        font.pixelSize: AccountStyle.core.size
                        background: Rectangle {
                            gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
                        }
                    }
                }

                MenuItem {
                    text: qsTr("New profile")
                    objectName: "newProfile"

                    onTriggered: popProfile.open()
                    font.family: AccountStyle.core.name
                    font.pixelSize: AccountStyle.core.size

                    background: Rectangle {
                        gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
                    }
                }

                MenuItem {
                    text: qsTr("Delete profile") + ": " + profileRepeater.current
                    objectName: "deleteProfile"
                    enabled: profileRepeater.current !== "Default"

                    signal s_deleteProfile(string name)
                    onTriggered: s_deleteProfile(profileRepeater.current)
                    font.family: AccountStyle.core.name
                    font.pixelSize: AccountStyle.core.size

                    background: Rectangle {
                        gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
                    }
                }

            }

            MenuItem {
                text: qsTr("&Quit")
                font.family: AccountStyle.core.name
                font.pixelSize: AccountStyle.core.size
                id: quitMenu

                indicator: AccountLabel {
                    text: quitShort.nativeText
                    anchors.rightMargin: 10
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }

                Shortcut {
                    id: quitShort
                    sequence: "CTRL+Q"
                    context: Qt.ApplicationShortcut
                    onActivated: quitMenu.clicked()
                }

                background: Rectangle {
                    gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
                }

                onClicked: {
                    mainWindow.close()
                }
            }
        }

        Menu {
            title: qsTr("Account")
            font.family: AccountStyle.core.name
            font.pixelSize: AccountStyle.core.size
            width: 210
            MenuItem {
                id: nAccountMenu
                text: qsTr("&New account")
                font.family: AccountStyle.core.name
                font.pixelSize: AccountStyle.core.size

                onClicked: {
                    adding(true)
                }

                indicator: AccountLabel {
                    text: accountAddShort.nativeText
                    anchors.rightMargin: 10
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }

                Shortcut {
                    id: accountAddShort
                    sequence: "CTRL+N"

                    onActivated: {
                        nAccountMenu.clicked()
                    }
                }

                background: Rectangle {
                    gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
                }
            }
            MenuItem {
                id: dAccountMenu
                text: qsTr("&Delete account")
                font.family: AccountStyle.core.name
                font.pixelSize: AccountStyle.core.size
                enabled: accountSelect.model && accountSelect.model.length > 0
                background: Rectangle {
                    gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
                }

                indicator: AccountLabel {
                    text: accountDelShort.nativeText
                    anchors.rightMargin: 10
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }

                Shortcut {
                    id: accountDelShort
                    sequence: "CTRL+SHIFT+D"

                    onActivated: {
                        dAccountMenu.clicked()
                    }
                }

                onClicked: {
                    deleteAccount.open()
                }
            }
        }

        Menu {
            title: qsTr("Current account")
            font.family: AccountStyle.core.name
            font.pixelSize: AccountStyle.core.size

            MenuItem {
                id: addEntryMenu
                text: qsTr("&Add transaction")
                font.family: AccountStyle.core.name
                font.pixelSize: AccountStyle.core.size
                enabled: dAccountMenu.enabled
                onClicked: {
                    if (enabled) {
                        adding(false)
                    }
                }

                indicator: AccountLabel {
                    text: addShort.nativeText
                    anchors.rightMargin: 10
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }

                Shortcut {
                    id: addShort
                    sequence: "CTRL+A"
                    context: Qt.ApplicationShortcut
                    onActivated: addEntryMenu.clicked()
                }

                background: Rectangle {
                    gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
                }
            }

            MenuItem {
                id: removeEntryMenu
                text: qsTr("&Remove transaction")
                font.family: AccountStyle.core.name
                font.pixelSize: AccountStyle.core.size

                enabled: table.currentId !== -1
                indicator: AccountLabel {
                    text: delShort.nativeText
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }

                Shortcut {
                    id: delShort
                    sequence: "CTRL+D"
                    context: Qt.ApplicationShortcut
                    onActivated: removeEntryMenu.clicked()
                }

                onClicked: {
                    if (enabled) {
                        remove(table.currentId)
                    }
                }

                background: Rectangle {
                    gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
                }
            }
        }

        background: Rectangle {
            gradient: AccountStyle.goldHeader
        }

        Menu {
            title: qsTr("?")
            font.family: AccountStyle.core.name
            font.pixelSize: AccountStyle.core.size

            MenuItem {
                text: qsTr("About")
                font.family: AccountStyle.core.name
                font.pixelSize: AccountStyle.core.size

                background: Rectangle {
                    gradient: AccountStyle.goldButton
                }

                onClicked: about.open()

                Popup {
                    id: about
                    objectName: "about"
                    height: 300
                    width: 700

                    property string text: "Unable to about"
                    ScrollView {
                        anchors.fill: parent
                        clip: true

                        AccountLabel {
                            text: about.text
                            anchors.centerIn: parent
                            font.family: AccountStyle.title.name
                            font.pixelSize: AccountStyle.title.size
                        }
                    }

                    background: AccountBackground {}

                    AccountButton {
                        text: qsTr("Ok")
                        onClicked: about.close()
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                    }
                }
            }

            MenuItem {
                text: qsTr("Licence")
                background: Rectangle {
                    gradient: AccountStyle.goldButton
                }

                onTriggered: aboutQt.open()

                Popup {
                    id: aboutQt
                    objectName: "licence"
                    height: 740
                    width: 740

                    property string text: "Unable to load licence"

                    ScrollView{
                        anchors.fill: parent
                        clip: true
                        AccountLabel {
                            anchors.centerIn: parent
                            clip: true
                            text: aboutQt.text
                            fontSizeMode: Text.Fit
                        }
                    }

                    background: AccountBackground {}

                    AccountButton {
                        text: qsTr("Ok")
                        onClicked: aboutQt.close()
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                    }
                }
            }

            MenuItem {
                text: qsTr("How to")
                background: Rectangle {
                    gradient: AccountStyle.goldButton
                }

                onTriggered: howto.open()

                Popup {
                    id: howto
                    objectName: "howto"
                    height: 740
                    width: 740
                    property string text: "Unable to load readme"

                    ScrollView {
                        anchors.fill: parent
                        clip: true
                        AccountLabel {
                            clip: true
                            text: howto.text
                            anchors.centerIn: parent
                            horizontalAlignment: Qt.Left
                        }
                    }
                    background: AccountBackground {}

                    AccountButton {
                        text: qsTr("Ok")
                        onClicked: howto.close()
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                    }
                }
            }
        }
    }

    background: AccountBackground {}

    header: Rectangle {
        height: mainWindow.height * .05
        color: "transparent"
        id: head
        objectName: "head"
        property string accountName: ""
        property var total: 0
        property var selectionTotal: 0

        RowLayout {
            anchors.left: parent.left
            anchors.right: accountSelect.left
            Layout.preferredHeight: parent.height

            AccountLabel {
                id: accountTitle
                objectName: "accountTitle"
                text: qsTr("Account") + ": " + head.accountName + " = " + head.total.value + "€"
                font.pixelSize: AccountStyle.title.size
                font.family: AccountStyle.title.name
                padding: 10

                color: accountSelect.model && accountSelect.model.length > 0 ? head.total.value > 0 ? "green" : "red" : "transparent"
                Layout.preferredWidth: parent.width * .5
            }
        }

        ComboBox {
            id: accountSelect
            objectName: "accountSelect"
            anchors.right: parent.right
            height: parent.height
            font.family: AccountStyle.core.name
            font.pixelSize: AccountStyle.core.size
            enabled: model.length > 0
            signal s_currentTextChange(string text)
            model: []

            ToolTip.text: qsTr("Select account")
            ToolTip.delay: 500
            ToolTip.visible: hovered

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                acceptedButtons: Qt.NoButton
            }
            Rectangle {
                gradient: AccountStyle.goldButton
                anchors.fill: parent
            }

            delegate: ItemDelegate {
                width: accountSelect.width

                contentItem: Rectangle {
                    gradient: AccountStyle.goldButton
                    anchors.fill: parent
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        acceptedButtons: Qt.NoButton
                    }
                    AccountLabel {
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

    CheckEstimated {
        objectName: "cEstimated"
    }

    SwipeView {
        id: swipeView
        objectName: "swipe"
        currentIndex: tabBar.currentIndex
        interactive: true

        anchors.fill: parent
        anchors.topMargin: mainWindow.height * 0.01
        anchors.bottomMargin: mainWindow.height * 0.01

        enabled: accountSelect.model.length > 0

        background: Rectangle {
            color: "transparent"
        }

        Page1Form {
            id: table
            objectName: "table"
        }

        PageView {
            id: graphSwipe
            Page2Form {
                id: graph
                
                anchors.fill: parent
            }

            PieCategory {
                id: pie

                anchors.fill: parent
                
            }

        }

        Popup {
            id: deleteAccount
            anchors.centerIn: swipeView

            width: labelDelete.width * 1.2
            height: (labelDelete.height + delOk.height + 3 * delOk.padding) * 1.1

            background: AccountBackground {}

            AccountLabel {
                id: labelDelete
                property string account: accountSelect.currentText
                text: qsTr("Delete") + " " + account + " ?"
                font.family: AccountStyle.title.name
                font.pixelSize: AccountStyle.title.size
                anchors.horizontalCenter: parent.horizontalCenter
            }

            AccountButton {
                id: delOk
                text: qsTr("Ok")
                anchors.top: labelDelete.bottom
                anchors.topMargin: padding

                onClicked: {
                    mainWindow.removeAccount(labelDelete.account)
                    deleteAccount.close()
                }
            }

            AccountButton {
                id: delCancel
                anchors.left: delOk.right
                anchors.leftMargin: padding
                anchors.top: labelDelete.bottom
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

        Repeater {
            objectName: "features"

            TabButton {
                id: listTabButton

                contentItem: AccountLabel {
                    text: modelData

                    color: tabBar.currentItem === listTabButton ? "black" : "darkseagreen"
                    font.family: AccountStyle.title.name
                    font.pixelSize: AccountStyle.title.size
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
}
