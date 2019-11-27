import QtQuick 2.11
import QtQuick.Controls 1.4
import QtQuick.Window 2.12
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.13

import "../Style" as S
import "../Transfert" as T
import "../CommonExpanse"

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
    signal openBudgetManager()
    signal s_closing()
    signal s_openFrequencyManager()
    
    id: mainWindow
    
    onClosing: s_closing()
    
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
            title: qsTr("Feature")
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size
            
            height: budgetListItem.height * 2//TOREMOVE
            MenuItem {
                id: budgetListItem
                text: qsTr("Budget list")
                onClicked: mainWindow.openBudgetManager()
                
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                
                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }
                
            }
            
            MenuItem {
                id: frequencyList
                text: qsTr("Frequency list")
                onClicked: mainWindow.s_openFrequencyManager()
                
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                
                background: Rectangle {
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                    
                }
            }
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
        
        Rectangle {}
        //        Page2Form {
        //            id: graph
        //            month: table.v_dateMonth
        //            year: table.v_dateYear
        //        }
        
        Rectangle {
            color: "transparent"

            Popup {
                id: popAddCommon

                onOpened: commonTitle.clear()
                signal s_accepted()

                GridLayout {
                    TextInput{
                        id: commonTitle
                        objectName: "commonTitle"
                        Layout.columnSpan: 4
                        Layout.row: 0
                        Layout.column: 1
                    }

                    Button {
                        objectName: "commonAddOk"
                        text: qsTr("Ok")
                        Layout.column: 2
                        Layout.row: 1
                        onClicked: {
                            popAddCommon.close()
                            popAddCommon.s_accepted()
                        }
                    }
                    Button {
                        text: qsTr("Cancel")
                        Layout.column: 3
                        Layout.row: 1

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
                        width: parent.width
                        height: parent.height * 0.93
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
                        }
                        Button {
                            width: parent.width * .49
                            height: parent.height
                            text: qsTr("Remove common expanse")
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
        
        TabButton {
            id: commonTabButton
            
            contentItem: Label {
                text: qsTr("Common expanse")
                horizontalAlignment: Qt.AlignHCenter
                color: tabBar.currentItem === commonTabButton ? "black" : "darkseagreen"
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
