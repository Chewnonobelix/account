import QtQuick 2.11
import Qt.labs.platform 1.1 as P

import QtQuick.Controls 1.4
import QtQuick.Window 2.12
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.13

import "../Style" as S
import "../Transfert" as T
import "../CommonExpanse"
import "../Budget"
import "../Frequency"
import "../MultiProfile"

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
    
    id: mainWindow
    
    onClosing: s_closing()


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
    
    S.AccountStyle {
        id: pageStyle
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
        font.family: pageStyle.core.name
        font.pixelSize: pageStyle.core.size
        
        objectName: "menuBar"
        Menu {
            title: qsTr("&File")

            width: 210
            delegate: MenuItem {
                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }
            }

            Menu {
                id: profileMenu
                objectName: "profileMenu"
                width: 210
                title: qsTr("Profile")
                signal s_profile(string name)
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size

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
                        font.family: pageStyle.core.name
                        font.pixelSize: pageStyle.core.size
                        background: Rectangle {
                            gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                        }
                    }
                }

                MenuItem {
                    text: qsTr("New profile")
                    objectName: "newProfile"

                    onTriggered: popProfile.open()
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size

                    background: Rectangle {
                        gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                    }
                }

                MenuItem {
                    text: qsTr("Delete profile") + ": " + profileRepeater.current
                    objectName: "deleteProfile"
                    enabled: profileRepeater.current !== "Default"

                    signal s_deleteProfile(string name)
                    onTriggered: s_deleteProfile(profileRepeater.current)
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size

                    background: Rectangle {
                        gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                    }
                }

            }

            MenuItem {
                text: qsTr("&Consolidate")
                objectName: "xmlMenu"
                //                checkable: true
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                enabled: false
                signal s_xml
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

                indicator: Label {
                    text: quitShort.nativeText
                    anchors.rightMargin: 10
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                }

                Shortcut {
                    id: quitShort
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
            width: 210
            MenuItem {
                id: nAccountMenu
                text: qsTr("&New account")
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size

                onClicked: {
                    adding(true)
                }

                indicator: Label {
                    text: accountAddShort.nativeText
                    anchors.rightMargin: 10
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                }

                Shortcut {
                    id: accountAddShort
                    sequence: "CTRL+N"

                    onActivated: {
                        nAccountMenu.clicked()
                    }
                }

                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }
            }
            MenuItem {
                id: dAccountMenu
                text: qsTr("&Delete account")
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                enabled: accountSelect.model && accountSelect.model.length > 0
                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }

                indicator: Label {
                    text: accountDelShort.nativeText
                    anchors.rightMargin: 10
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
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
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size

            MenuItem {
                id: addEntryMenu
                text: qsTr("&Add transaction")
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                enabled: dAccountMenu.enabled
                onClicked: {
                    if (enabled) {
                        adding(false)
                    }
                }

                indicator: Label {
                    text: addShort.nativeText
                    anchors.rightMargin: 10
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                }

                Shortcut {
                    id: addShort
                    sequence: "CTRL+A"
                    context: Qt.ApplicationShortcut
                    onActivated: addEntryMenu.clicked()
                }

                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }
            }

            MenuItem {
                id: removeEntryMenu
                text: qsTr("&Remove transaction")
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size

                enabled: table.currentId !== -1
                indicator: Label {
                    text: delShort.nativeText
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
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
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }
            }
        }

        background: Rectangle {
            gradient: pageStyle.goldHeader
        }

        Menu {
            title: qsTr("?")
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size

            MenuItem {
                text: qsTr("About")
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size

                background: Rectangle {
                    gradient: pageStyle.goldButton
                }

                onClicked: about.open()

                Popup {
                    id: about
                    height: 200
                    width: 600
                    Label {
                        anchors.fill: parent
                        text: qsTr("This application is made by Arnaud DUHAMEL
Current Version beta 0.9")
                        font.family: pageStyle.title.name
                        font.pixelSize: pageStyle.title.size
                        verticalAlignment: Text.AlignVCenter
                        fontSizeMode: Text.Fit
                    }

                    background: Rectangle {
                        anchors.fill: parent
                        gradient: pageStyle.backgroundGradient
                        border.color: "gold"
                    }

                    Button {
                        text: qsTr("Ok")
                        onClicked: about.close()
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom

                        background: Rectangle {
                            anchors.fill: parent
                            gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                        }
                    }
                }
            }

            MenuItem {
                text: qsTr("Licence")
                background: Rectangle {
                    gradient: pageStyle.goldButton
                }

                onTriggered: aboutQt.open()

                Popup {
                    id: aboutQt
                    height: 200
                    width: 600
                    Label {
                        anchors.fill: parent
                        text: qsTr("This application is made with Qt 5.12 and QML")
                        font.family: pageStyle.title.name
                        font.pixelSize: pageStyle.title.size
                        verticalAlignment: Text.AlignVCenter
                        fontSizeMode: Text.Fit
                    }

                    background: Rectangle {
                        anchors.fill: parent
                        gradient: pageStyle.backgroundGradient
                        border.color: "gold"
                    }

                    Button {
                        text: qsTr("Ok")
                        onClicked: aboutQt.close()
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom

                        background: Rectangle {
                            anchors.fill: parent
                            gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                        }
                    }
                }
            }
        }
    }

    background: Rectangle {
        id: backRect
        anchors.fill: parent
        gradient: pageStyle.backgroundGradient
    }

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

            Label {
                id: accountTitle
                objectName: "accountTitle"
                text: qsTr("Account") + ": " + head.accountName + " = " + head.total.value + "€"
                font.pixelSize: pageStyle.title.size
                font.family: pageStyle.title.name
                padding: 10
                fontSizeMode: Text.Fit
                color: accountSelect.model && accountSelect.model.length > 0 ? head.total.value > 0 ? "green" : "red" : "transparent"
                Layout.preferredWidth: parent.width * .5
            }

            Label {
                id: selectLabel
                fontSizeMode: Text.Fit
                text: qsTr(
                          "Selection total") + ": " + head.selectionTotal.value + " €"
                font.pixelSize: pageStyle.title.size
                font.family: pageStyle.title.name
                padding: 10
                color: accountSelect.model && accountSelect.model.length
                       > 0 ? head.selectionTotal.value > 0 ? "green" : "red" : "transparent"
                Layout.preferredWidth: parent.width * .5
            }
        }

        ComboBox {
            id: accountSelect
            objectName: "accountSelect"
            anchors.right: parent.right
            height: parent.height
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size
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
        objectName: "swipe"
        currentIndex: tabBar.currentIndex

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

        Page2Form {
            id: graph
            month: table.v_dateMonth
            year: table.v_dateYear
        }

//        BudgetManager {
//            id: bm
//            visible: true
//            objectName: "budgetManager"
//        }

//        FrequencyManager{
//            id: fm
//            visible: true
//            objectName: "frequencyManager"
//        }

//        CommonExpanseManager {
//            id: cm
//        }

        Popup {
            id: deleteAccount
            anchors.centerIn: swipeView

            width: labelDelete.width * 1.2
            height: (labelDelete.height + delOk.height + 3 * delOk.padding) * 1.1

            background: Rectangle {
                anchors.fill: parent
                gradient: pageStyle.backgroundGradient
                border.color: "gold"
            }

            Label {
                id: labelDelete
                property string account: accountSelect.currentText
                text: qsTr("Delete") + " " + account + " ?"
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                anchors.horizontalCenter: parent.horizontalCenter
                
            }

            Button {
                id: delOk
                text: qsTr("Ok")
                anchors.top: labelDelete.bottom
                anchors.topMargin: padding
                font.pixelSize: pageStyle.core.size
                font.family: pageStyle.core.name

                onClicked: {
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
                anchors.top: labelDelete.bottom
                anchors.topMargin: padding
                font.pixelSize: pageStyle.core.size
                font.family: pageStyle.core.name

                text: qsTr("Cancel")

                onClicked: {
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

        Repeater {
            objectName: "features"
            model: [qsTr("List"), qsTr("Graph"), qsTr("Budget"), qsTr("Frequency"), qsTr("Common expanse")]
            TabButton {
                id: listTabButton
    
                contentItem: Label {
                    text: modelData
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    color: tabBar.currentItem === listTabButton ? "black" : "darkseagreen"
                    font.family: pageStyle.title.name
                    font.pixelSize: pageStyle.title.size
                    fontSizeMode: Text.Fit
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
