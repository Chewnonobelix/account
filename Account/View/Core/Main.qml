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
        width: 600
        height: 400
        anchors.centerIn: parent
    }
    
    menuBar: MenuBar {
        font.family: pageStyle.core.name
        font.pixelSize: pageStyle.core.size
        
        objectName: "menuBar"
        Menu {
            title: qsTr("&File")

            Menu {
                id: profileMenu
                objectName: "profileMenu"
                title: qsTr("Profile")
                signal s_profile(string name)
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
//                background: Rectangle {
//                    gradient: pageStyle.goldHeader
//                }

                Repeater {
                    objectName: "profileRepeater"
                    MenuItem {
                        text: modelData
                        autoExclusive: true
                        checkable: true
                        onTriggered: profileMenu.s_profile(text)
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
                enabled: accountSelect.model.length > 0
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
                        fontSizeMode: Text.Fit0
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
        property string accountName
        property var total
        property var selectionTotal
        
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
                color: accountSelect.model.length
                       > 0 ? head.total.value > 0 ? "green" : "red" : "transparent"
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
                color: accountSelect.model.length
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
            enabled: accountSelect.model.length > 0
            signal s_currentTextChange(string text)
            
            
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
        
        BudgetManager {
            objectName: "budgetManager"
        }
        
        FrequencyManager{
            objectName: "frequencyManager"
        }
        
        Rectangle {
            color: "transparent"
            objectName: "commonRect"
            
            Popup {
                id: popAddCommon
                objectName: "popAddCommon"
                onOpened: commonTitle.clear()
                signal s_accepted(string name)
                
                anchors.centerIn: parent
                background: Rectangle {
                    gradient: pageStyle.backgroundGradient
                    border.color: "gold"
                }
                
                GridLayout {
                    anchors.fill: parent
                    Label {
                        text: qsTr("Session name")
                        Layout.row: 0
                        Layout.column: 0

                        font.family: pageStyle.title.name
                        font.pixelSize: pageStyle.title.size
                        fontSizeMode: Text.Fit
                    }
                    
                    TextField{
                        id: commonTitle
                        objectName: "commonTitle"
                        Layout.columnSpan: 3
                        Layout.row: 0
                        Layout.column: 1
                        Layout.fillWidth: true
                        font.family: pageStyle.core.name
                        font.pixelSize: pageStyle.core.size

                        property bool valid: text.length > 0


                        background: Rectangle {
                            border.color: parent.valid ? "blue" : "red"
                        }
                    }
                    
                    Button {
                        objectName: "commonAddOk"
                        text: qsTr("Ok")
                        Layout.column: 2
                        Layout.row: 1
                        
                        onClicked: {
                            popAddCommon.close()
                            popAddCommon.s_accepted(commonTitle.text)
                        }

                        background: Rectangle {
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                cursorShape: Qt.PointingHandCursor
                            }

                            gradient: pageStyle.goldButton
                        }
                    }
                    Button {
                        text: qsTr("Cancel")
                        Layout.column: 3
                        Layout.row: 1

                        background: Rectangle {
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                cursorShape: Qt.PointingHandCursor
                            }

                            gradient: pageStyle.goldButton
                        }

                        onClicked: popAddCommon.close()
                    }
                }
            }
            
            Row {
                anchors.fill: parent
                spacing: width * 0.02
                Column{
                    height: parent.height
                    width: parent.width * 0.20
                    spacing: height * .02
                    ListView {
                        id: listCommon
                        objectName: "listCommon"
                        width: parent.width
                        height: parent.height * 0.93
                        clip: true
                        
                        Component.onCompleted: {
                            currentModel = Qt.binding(function() {return currentIndex > -1 ? model[currentIndex] : null })
                            commonExpanse.model = Qt.binding(function() {return currentModel})
                            commonExpanse.enabled = Qt.binding(function() {return currentIndex > -1})
                            removeCommon.enabled = Qt.binding(function() {return currentIndex > -1})
                        }

                        property var currentModel
                        Rectangle{
                            anchors.fill: parent
                            color: "transparent"
                            border.color: "gold"
                        }
                        
                        highlight: Rectangle {
                            gradient: pageStyle.calSelect
                        }
                        
                        highlightMoveDuration: 0
                        
                        delegate: Label {
                            text: modelData.title
                            height: listCommon.height * 0.07
                            width: listCommon.width
                            
                            horizontalAlignment: Qt.AlignHCenter
                            verticalAlignment: Qt.AlignVCenter
                            
                            fontSizeMode: Text.Fit
                            font.family: pageStyle.core.name
                            font.pixelSize: pageStyle.core.size
                            
                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: listCommon.currentIndex = listCommon.currentIndex === index ? -1 : index
                            }
                        }
                    }
                    
                    Row {
                        width: parent.width
                        height: parent.height * 0.05
                        spacing: width * 0.02
                        Button {
                            width: parent.width * .49
                            height: parent.height
                            text: qsTr("Add common expanse")
                            onClicked: popAddCommon.open()
                            
                            font.family: pageStyle.core.name
                            font.pixelSize: pageStyle.core.size

                            background: Rectangle {
                                MouseArea{
                                    acceptedButtons: Qt.NoButton
                                    cursorShape: Qt.PointingHandCursor
                                    anchors.fill: parent
                                }

                                gradient: pageStyle.goldButton
                            }
                        }
                        Button {
                            id: removeCommon
                            objectName: "removeCommon"
                            width: parent.width * .49
                            height: parent.height
                            text: qsTr("Remove common expanse")
                            
                            font.family: pageStyle.core.name
                            font.pixelSize: pageStyle.core.size
                            
                            signal s_remove(int id)
                            onClicked: s_remove(listCommon.currentModel.id)

                            background: Rectangle {
                                MouseArea{
                                    acceptedButtons: Qt.NoButton
                                    cursorShape: Qt.PointingHandCursor
                                    anchors.fill: parent
                                }
                                gradient: pageStyle.goldButton
                            }
                        }
                    }
                }
                
                CommonExpanseView {
                    id: commonExpanse
                    objectName: "common"
                    height: parent.height
                    width: parent.width * .78
                }
            }
        }
        
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
                anchors.horizontalCenter: parent.width / 2
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
        
        TabButton {
            id: listTabButton
            
            contentItem: Label {
                text: qsTr("List")
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
        TabButton {
            id: graphTabButton
            
            contentItem: Label {
                text: qsTr("Graph")
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                color: tabBar.currentItem === graphTabButton ? "black" : "darkseagreen"
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
        
        TabButton {
            id: budgetTabButton
            
            contentItem: Label {
                text: qsTr("Budget")
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                color: tabBar.currentItem === budgetTabButton ? "black" : "darkseagreen"
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

        TabButton {
            id: frequencyTabButton
            
            contentItem: Label {
                text: qsTr("Frequency")
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                color: tabBar.currentItem === frequencyTabButton ? "black" : "darkseagreen"
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
        
        TabButton {
            id: commonTabButton
            
            contentItem: Label {
                text: qsTr("Common expanse")
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                wrapMode: Text.WordWrap
                color: tabBar.currentItem === commonTabButton ? "black" : "darkseagreen"
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
