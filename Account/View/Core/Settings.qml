import QtQuick 2.14
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "../Style"

Dialog {    
    id: root
    title: qsTr("Settings")
    standardButtons: Dialog.Save | Dialog.Cancel | DialogButtonBox.Apply
    
    width: 600
    height: 400
    
    anchors.centerIn: parent
    clip: true
    
    header: AccountHeader {
        height: root.height * .15
        width: root.width
        text: root.title
    }

    background: AccountBackground{}
    
    footer: DialogButtonBox {
        alignment: Qt.AlignRight
        background: Rectangle {
            color: "transparent"
        }

        delegate: AccountButton {}
    }


    ScrollView {
        anchors.fill: parent
        clip: true

        Column {
            spacing: root.height * 0.02
            clip: true

            anchors.rightMargin: root.width * 0.02
            anchors.leftMargin: root.width * 0.02

            GroupBox {
                id: general
                label: AccountHeader {
                    text: qsTr("General")
                    width: general.width
                    height: root.height * 0.10
                }

                width: root.width * 0.94

                background: AccountBackground{invisible: true}
                
                GridLayout {
                    columns: 2
                    columnSpacing: root.width * 0.02
                    rowSpacing: root.height * 0.02

                    anchors.fill: parent
                    anchors.topMargin: root.height * 0.02
                    anchors.bottomMargin: root.height * 0.02
                    anchors.rightMargin: root.width * 0.02
                    anchors.leftMargin: root.width * 0.02

                    AccountLabel {
                        text: qsTr("Language")
                        font.family: AccountStyle.title.name
                        Layout.preferredHeight: root.height * 0.07
                    }
                    
                    AccountComboBox {
                        objectName: "language"
                        Layout.preferredHeight: root.height * 0.07
                        ToolTip.text: qsTr("Select language")
                    }
                }
            }
            
            GroupBox {
                id: features

                label: AccountHeader {
                    text: qsTr("Features")
                    width: features.width
                    height: root.height * 0.10
                }
                width: root.width * 0.94

                background: AccountBackground{
                    invisible: true
                }
                
                GridLayout {
                    columns: 1
                    columnSpacing: root.width * 0.02
                    rowSpacing: root.height * 0.02

                    anchors.fill: parent
                    anchors.topMargin: root.height * 0.02
                    anchors.bottomMargin: root.height * 0.02
                    anchors.rightMargin: root.width * 0.02
                    anchors.leftMargin: root.width * 0.02

                    
                    AccountCheckBox {
                        objectName: "budget"
                        Layout.preferredHeight: root.height * 0.07
                        text: qsTr("Budget")
                        font.family: AccountStyle.title.name
                        
                        ToolTip.text: (checked ? qsTr("Disable") : qsTr("Enable")) + " " + qsTr("budget")
                    }
                                        
                    AccountCheckBox {
                        objectName: "frequency"
                        Layout.preferredHeight: root.height * 0.07
                        text: qsTr("Frequency")
                        ToolTip.text: (checked ? qsTr("Disable") : qsTr("Enable")) + " " + qsTr("frequency")
                        font.family: AccountStyle.title.name
                    }
                                        
                    AccountCheckBox {
                        objectName: "common"
                        Layout.preferredHeight: root.height * 0.07
                        text: qsTr("Common Expanse")
                        font.family: AccountStyle.title.name
                        ToolTip.text: (checked ? qsTr("Disable") : qsTr("Enable")) + " " + qsTr("common expanse")
                    }
                }
            }
            
            Component {
                id: db
                AccountComboBox {
                    textRole: "name"
                    valueRole: "value"
                    ToolTip.text: qsTr("Select database")
                    
                    model: ListModel {
                        ListElement {
                            name: qsTr("Xml")
                            value: "ControllerXMLMulti"
                        }
                        
                        ListElement {
                            name: qsTr("Sql")
                            value: "ControllerDB"
                        }
                    }
                }
            }
            
            GroupBox {
                id: database

                width: root.width * 0.94

                label: AccountHeader {
                    text: qsTr("Database")
                    width:database.width
                    
                    height: root.height * 0.10
                }
                
                background: AccountBackground{invisible: true}
                
                GridLayout {
                    columns: 2
                    columnSpacing: root.width * 0.02
                    rowSpacing: root.height * 0.02

                    anchors.fill: parent
                    anchors.topMargin: root.height * 0.02
                    anchors.bottomMargin: root.height * 0.02
                    anchors.rightMargin: root.width * 0.02
                    anchors.leftMargin: root.width * 0.02

                    AccountLabel {
                        Layout.column: 0
                        Layout.row: 0
                        text: qsTr("Main database")
                        font.family: AccountStyle.title.name
                        Layout.preferredHeight: root.height * 0.07
                        Layout.preferredWidth:  database.width * .40
                        horizontalAlignment: Qt.AlignLeft

                    }
                    
                    Loader {
                        Layout.column: 1
                        Layout.row: 0
                        objectName: "primary"
                        active: true
                        sourceComponent: db
                        Layout.preferredHeight: root.height * 0.07
                        Layout.preferredWidth: root.width * 0.50
                    }
                    
                    
                    
                    AccountCheckBox {
                        Layout.column: 0
                        Layout.row: 1
                        
                        id: secondadyEnable
                        objectName: "useBackup"
                        text: qsTr("Backup database")
                        checked: true
                        Layout.preferredHeight: root.height * 0.07
                        Layout.preferredWidth:  database.width * .40
                        font.family: AccountStyle.title.name
                        LayoutMirroring.enabled: true
                    
                        ToolTip.text: (checked ? qsTr("Disable") : qsTr("Enable")) + " " + qsTr("backup database")
                    }
                    
                    
                    Loader {
                        Layout.column: 1
                        Layout.row: 1
                        objectName: "backup"
                        sourceComponent: db
                        active: secondadyEnable.checked
                        Layout.preferredHeight: root.height * 0.07
                        Layout.preferredWidth: root.width * 0.50
                        id: backup
                        ToolTip.text: qsTr("Select backup database")
                    }

                    AccountCheckBox {
                        text: qsTr("Auto backup")
                        objectName: "autoconsolidate"
                        visible: secondadyEnable.checked
                        LayoutMirroring.enabled: true
                        Layout.preferredHeight: root.height * 0.07
                        Layout.preferredWidth:  database.width * .40
                        font.family: AccountStyle.title.name
                        ToolTip.text: (checked ? qsTr("Disable") : qsTr("Enable")) + " " +qsTr("automatic backup")
                    }

                    Row {
                        Layout.row: 2
                        Layout.column: 1
                        spacing: root.width * 0.02
                        AccountButton {
                            objectName: "saveBackup"
                            text: qsTr("Save backup")
                            enabled: secondadyEnable.checked
                            Layout.preferredHeight: root.height * 0.07
                        }

                        AccountButton {
                            text: qsTr("Restore backup")

                            Layout.preferredHeight: root.height * 0.07

                            onClicked: restoreDialog.open()

                            FileDialog {
                                id: restoreDialog
                                objectName: "restoreDialog"
                                selectExisting: true
                                nameFilters: ["Account backup (*.bckx *.bcks)"]
                                title: qsTr("Restore backup")
                                signal s_restore(string file)
                                onAccepted: s_restore(fileUrl)
                            }
                        }
                    }
                }
            }

            GroupBox {
                id: sync
                label: AccountHeader {
                    text: qsTr("Syncronization")
                    width: sync.width
                    height: root.height * 0.10
                }
                width: root.width * 0.94
                height: root.height
                SynchronizationGlobal {
                    anchors.fill: parent
                    objectName: "syncronization"
                }
            }
        }
    }
}
