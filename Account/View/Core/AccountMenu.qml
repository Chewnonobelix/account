import QtQuick 2.15
import QtQuick.Controls 2.15

import "../Style"
import "../MultiProfile"

Drawer {
    edge: Qt.LeftEdge
    interactive: true
    id: drawer

    NewProfile {
        id: popProfile
        objectName: "popProfile"
        width: 300
        height: 100
        anchors.centerIn: parent
    }


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
                            _settings.open()
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

                        onClicked:  _main.deleteProfile(drawer.currentProfile)
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
                                        _main.changeProfile(text)
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
