import QtQuick 2.11
import Qt.labs.platform 1.1 as P

//import QtQuick.Controls 1.4
import QtQuick.Window 2.12
import QtQuick.Controls 2.4
//import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs
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
    
    width: 1920
    height: 1080
        
    id: mainWindow
    
    flags: Qt.Window | Qt.FramelessWindowHint
    onClosing: _main.close()
    
    AccountMenu {
        height: mainWindow.height
        width: mainWindow.width / 4
        id: drawer
        objectName: "drawer"
    }

    Settings {
        objectName: "settings"
    }
    
//    onVisibilityChanged: {
//        _categoryModel.onUpdateCategory()
//        if(visibility === Window.Minimized)
//            hide()
//    }
    
    QuickAdding {
        id: quick
        objectName: "quick"
        x: Screen.width - width
        y: Screen.desktopAvailableHeight - height
                
        onVisibleChanged: if(visible) _main.quickOpen()()
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
        _main.loadAccount()
//        _main.buildModel()
        //        showMaximized()
    }
    
    Transfert {
        id: tranfert
        objectName: "transfert"
        
        anchors.centerIn: parent
        width: mainWindow.width * .75
        height: mainWindow.height * .50
    }
    
    Popup {
        id: about
        objectName: "about"
        height: 300
        width: 700
        
        onOpened: text = _main.about()
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
        
        onOpened: text = _main.licence()

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
        onOpened: text = _main.readme()

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
        onActivated: _main.openTransfert()
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
        onActivated: _main.remove(table.currentEntry.id)
        
    }
    
    Shortcut {
        id: addShort
        sequence: "CTRL+A"
        context: Qt.ApplicationShortcut
        onActivated: _main.add(false)
    }
    
    Shortcut {
        id: addAccountShort
        sequence: "CTRL+SHIFT+A"
        context: Qt.ApplicationShortcut
        onActivated:  {
            _main.add(true)
        }        
    }
    
    Shortcut {
        id: quitShort
        sequence: "CTRL+Q"
        context: Qt.ApplicationShortcut
        onActivated: mainWindow.close()
    }
    
    
    
    background: AccountBackground {}
    
    property bool isMaximazed: false
    header: Rectangle {
        height: mainWindow.height * .05
        color: "transparent"
        id: head
        objectName: "head"
        property string accountName: accountSelect.currentText
        property var total: 0
        property var selectionTotal: 0

        Connections {
            target: _main

            function onTotaleChanged(total) {
                head.total = total
            }
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: !pressed ?  Qt.OpenHandCursor : Qt.ClosedHandCursor
            
            property bool holding: false
            property var clickPos: Qt.point(1,1)
            property var old: Qt.point(0,0)
            onPressed: (mouse) => {
                clickPos = Qt.point(mouse.x, mouse.y)
            }
            
            onPositionChanged: {
                if(!isMaximazed) {
                    var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
                    mainWindow.x += delta.x
                    mainWindow.y += delta.y
                }
            }
            
            onDoubleClicked: {
                if(!isMaximazed) {
                    old = Qt.point(mainWindow.x, mainWindow.y)
                    showMaximized()
                    mainWindow.x = Screen.virtualX
                    mainWindow.y = Screen.virtualY
                }
                else {
                    showNormal()
                    mainWindow.x = old.x
                    mainWindow.y = old.y
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
            Connections {
                target: _main

                function onCurrentAccountChanged(account) {
                    for(var i = 0; i < accountSelect.model.length; i++) {
                        if(accountSelect.model[i] === account) {
                            accountSelect.currentIndex = i
                        }
                    }
                }

                function onAccountChanged(list) {
                    accountSelect.model = list
                }
            }

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
                _main.accountChange(currentText)
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
        
        Connections {
            target: _main
            function onFeaturesChanged(list) {
                for(var i = 2; i < swipeView.count; i++) {
                    swipeView.takeItem(i)
                }

                for(var it in list) {
                    swipeView.addItem(list[it])
                }
            }
        }

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
                    _main.deleteAccount(labelDelete.account)
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
        
        Connections {
            target: _main

            function onFeaturesListChanged(list) {
                features.model = list
            }
        }

        Repeater {
            id: features
            
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
