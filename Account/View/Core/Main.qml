import QtQuick 2.11
import Qt.labs.platform 1.1 as P

import QtQuick.Controls 1.4
import QtQuick.Window 2.12
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.13

import "../Style"
import "../Transfert"
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
    
    flags: Qt.FramelessWindowHint
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
        //        showMaximized()
    }
    
    Transfert {
        id: tranfert
        objectName: "transfert"
        
        anchors.centerIn: parent
        width: mainWindow.width * .75
        height: mainWindow.height * .50
        onOpened: {
            addAccount(accountSelect.model)
        }
    }
    
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

    Shortcut {
        id: transfertShortcut
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
    
    MouseArea {
        height: mainWindow.height
        width: mainWindow.width*0.01
        anchors.left: parent.left
        z: 10
        acceptedButtons: Qt.NoButton

        id: drawerArea
        hoverEnabled: true
        onContainsMouseChanged: {
            if(containsMouse)
                drawer.open()
            
        }
    }
    
    Shortcut {
        id: accountDelShort
        sequence: "CTRL+SHIFT+D"

        onActivated: {
            deleteAccount.open()
        }
    }

    Shortcut {
        id: delShort
        sequence: "CTRL+D"
        context: Qt.ApplicationShortcut
        onActivated: remove(table.currentId)

    }

    Shortcut {
        id: addShort
        sequence: "CTRL+A"
        context: Qt.ApplicationShortcut
        onActivated: adding(false)
    }

    Shortcut {
        id: addAccountShort
        sequence: "CTRL+SHIFT+A"
        context: Qt.ApplicationShortcut
        onActivated: adding(true)

    }

    Shortcut {
        id: quitShort
        sequence: "CTRL+Q"
        context: Qt.ApplicationShortcut
        onActivated: mainWindow.close()
    }


    Drawer {
        edge: Qt.LeftEdge
        height: mainWindow.height
        width: mainWindow.width / 4
        interactive: true
        id: drawer
        objectName: "drawer"

        property var profileModel
        property string currentProfile


        signal s_profile(string profile)
        signal s_deleteProfile(string name)
        
        AccountBackground {
            anchors.fill: parent

            Column {
                anchors.fill: parent
                spacing: 0

                AccountMenuButton {
                    height: drawer.height*0.1
                    width: drawer.width*.99
                    text: qsTr("&File")

                    onClicked: {
                        load.sourceComponent = filemenu
                        load.active = true
                    }
                }

                AccountMenuButton {
                    height: drawer.height*0.1
                    width: drawer.width*.99
                    text: qsTr("&Account")

                    onClicked: {
                        load.sourceComponent = accountmenu
                        load.active = true
                    }

                }

                AccountMenuButton {
                    text: "?"

                    height: drawer.height*0.1
                    width: drawer.width*.99
                    onClicked: {
                        load.sourceComponent = aboutmenu
                        load.active = true
                    }
                }
            }

            Loader {
                id: load
                active: false
                anchors.fill: parent
            }

            Component {
                id: filemenu
                AccountBackground {
                    Column {
                        anchors.fill: parent
                        spacing: 0
                        AccountMenuButton {
                            text: "<="
                            onClicked: load.active = false
                            height: drawer.height*0.1
                            width: drawer.width*.99
                        }

                        AccountMenuButton {
                            text: qsTr("Settings")
                            onClicked: {
                                load.active = false
                                s_openSetting()
                            }
                            height: drawer.height*0.1
                            width: drawer.width*.99
                        }

                        AccountMenuButton {
                            id: profileMenu
                            objectName: "profileMenu"
                            height: drawer.height*0.1
                            width: drawer.width*.99
                            text: qsTr("Profile")
                            signal s_profile(string name)
                            onClicked: {
                                profileloader.active = true
                            }
                        }

                        AccountMenuButton {
                            text: qsTr("New profile")
                            onClicked: {
                                load.active = false
                                popProfile.open()
                            }
                            height: drawer.height*0.1
                            width: drawer.width*.99
                        }

                        AccountMenuButton {
                            text: qsTr("Delete profile") + ": " + drawer.currentProfile
                            objectName: "deleteProfile"
                            enabled: drawer.currentProfile !== "Default"

                            onClicked:  drawer.s_deleteProfile(drawer.currentProfile)
                            height: drawer.height*0.1
                            width: drawer.width*.99
                        }

                        AccountMenuButton {
                            text: qsTr("&Quit")
                            indicator: AccountLabel {
                                text: quitShort.nativeText
                                anchors.rightMargin: 10
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            id: quitMenu

                            icon.source: "qrc:/Style/exit.png"
                            onClicked:  {
                                quitShort.activated()
                            }
                            height: drawer.height*0.1
                            width: drawer.width*.99
                        }
                    }

                    Loader {
                        anchors.fill: parent
                        active: false
                        id: profileloader
                        sourceComponent: AccountBackground {
                            Column {
                                AccountMenuButton {
                                    text: "<="
                                    onClicked: profileloader.active = false
                                    height: drawer.height*0.1
                                    width: drawer.width*.99
                                }

                                Repeater {
                                    id: profileRepeater
                                    objectName: "profileRepeater"
                                    property string current: drawer.currentProfile
                                    model: drawer.profileModel

                                    AccountMenuButton {
                                        text: modelData
                                        checked: modelData === profileRepeater.current
                                        autoExclusive: true
                                        checkable: true
                                        onClicked: {
                                            drawer.currentProfile = text
                                            drawer.s_profile(text)
                                        }
                                        height: drawer.height*0.1
                                        width: drawer.width*.99
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Component {
                id: aboutmenu

                AccountBackground {
                    Column {
                        spacing: 0
                        anchors.fill: parent
                        AccountMenuButton {
                            text: "<="
                            onClicked: load.active = false
                            height: drawer.height*0.1
                            width: drawer.width*.99
                        }

                        AccountMenuButton {
                            text: qsTr("About")

                            onClicked: about.open()

                            height: drawer.height*0.1
                            width: drawer.width*.99
                        }

                        AccountMenuButton {
                            text: qsTr("Licence")

                            onClicked: aboutQt.open()


                            height: drawer.height*0.1
                            width: drawer.width*.99
                        }

                        AccountMenuButton {
                            text: qsTr("How to")
                            onClicked: howto.open()


                            height: drawer.height*0.1
                            width: drawer.width*.99
                        }
                    }
                }
            }

            Component {
                id: accountmenu

                AccountBackground {
                    Column {
                        AccountMenuButton {
                            text: "<="
                            onClicked: load.active = false
                            height: drawer.height*0.1
                            width: drawer.width*.99
                        }

                        AccountMenuButton {
                            id: nAccountMenu

                            text: qsTr("&New account")
                            height: drawer.height*0.1
                            width: drawer.width*.99

                            onClicked: {
                                addAccountShort.activated()
                            }

                            indicator: AccountLabel {
                                text: addAccountShort.nativeText
                                anchors.rightMargin: 10
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }

                        AccountMenuButton {
                            id: dAccountMenu
                            text: qsTr("&Delete account")
                            height: drawer.height*0.1
                            width: drawer.width*.99

                            enabled: accountSelect.model && accountSelect.model.length > 0

                            indicator: AccountLabel {
                                text: accountDelShort.nativeText
                                anchors.rightMargin: 10
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                            }


                            onClicked: {
                                accountDelShort.activated()
                            }
                        }

                        AccountMenuButton {
                            id: addEntryMenu
                            text: qsTr("&Add transaction")
                            height: drawer.height*0.1
                            width: drawer.width*.99

                            enabled: dAccountMenu.enabled
                            onClicked: {
                                if (enabled) {
                                    addShort.activated()
                                }
                            }

                            indicator: AccountLabel {
                                text: addShort.nativeText
                                anchors.rightMargin: 10
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                            }

                        }

                        AccountMenuButton {
                            id: removeEntryMenu
                            text: qsTr("&Remove transaction")
                            height: drawer.height*0.1
                            width: drawer.width*.99

                            enabled: table.currentId !== -1
                            indicator: AccountLabel {
                                text: delShort.nativeText
                                anchors.leftMargin: 10
                                anchors.rightMargin: 10
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                            }


                            onClicked: {
                                if (enabled) {
                                    delShort.activated()
                                }
                            }
                        }

                        AccountMenuButton {
                            text: qsTr("&Transfert")
                            height: drawer.height*0.1
                            width: drawer.width*.99

                            indicator: AccountLabel {
                                text: transfertShortcut.nativeText
                                anchors.leftMargin: 10
                                anchors.rightMargin: 10
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                clip: true
                            }


                            onClicked: {
                                transfertShortcut.activated()
                            }
                        }
                    }
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            z: 11
            acceptedButtons: Qt.NoButton

            hoverEnabled: true
            onContainsMouseChanged: {
                if(!containsMouse) {
                    load.active = false
                    drawer.close()
                }

            }
        }
    }

    background: AccountBackground {}

    property bool isMaximazed: false
    header: Rectangle {
        height: mainWindow.height * .05
        color: "transparent"
        id: head
        objectName: "head"
        property string accountName: ""
        property var total: 0
        property var selectionTotal: 0
        MouseArea {
            anchors.fill: parent
            cursorShape: !pressed ?  Qt.OpenHandCursor : Qt.ClosedHandCursor

            property bool holding: false
            property var clickPos: Qt.point(1,1)
            property var old: Qt.point(0,0)
            onPressed: {
                clickPos = Qt.point(mouse.x, mouse.y)
            }

            onPositionChanged: {
                var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
                mainWindow.x += delta.x
                mainWindow.y += delta.y
            }

            onDoubleClicked: {
                if(!isMaximazed) {
                    old = Qt.point(mainWindow.x, mainWindow.y)
                    mainWindow.x = 0
                    mainWindow.y = 0
                    showMaximized()
                }
                else {
                    mainWindow.x = old.x
                    mainWindow.y = old.y
                    showNormal()
                }

                isMaximazed = !isMaximazed
            }
        }

        RowLayout {
            anchors.left: parent.left
            anchors.right: accountSelect.left
            Layout.preferredHeight: parent.height

            AccountLabel {
                id: accountTitle
                objectName: "accountTitle"
                text: qsTr("Account") + ": " + head.accountName + " = " + head.total.value + Qt.locale().currencySymbol(Locale.CurrencySymbol)
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

        CommonGraph {
            id: graph
            objectName: "graph"
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
