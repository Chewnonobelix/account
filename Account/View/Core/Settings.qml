import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "../Style"

Dialog {    
    id: root
    title: qsTr("Settings")
    standardButtons: Dialog.Save | Dialog.Cancel
    
    width: 600
    height: 400
    
    anchors.centerIn: parent
    clip: true
    
    header: AccountHeader {
        height: root.height * .10
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
            anchors.fill: parent
            
            GroupBox {
                width: parent.width
                
                id: general
                label: AccountHeader {
                    text: qsTr("General")
                    width: general.width
                    height: root.height * 0.10
                }
                
                background: AccountBackground{invisible: true}
                
                GridLayout {
                    columns: 2
                    
                    AccountLabel {
                        text: qsTr("Language")
                        font.family: AccountStyle.title.name
                    }
                    
                    AccountComboBox {
                        objectName: "language"
                    }
                }
            }
            
            GroupBox {
                id: features
                width: parent.width
                label: AccountHeader {                    
                    text: qsTr("Features")
                    width: features.width
                    height: root.height * 0.10
                }
                
                background: AccountBackground{invisible: true}
                
                GridLayout {
                    columns: 2
                    AccountLabel {
                        text: qsTr("Budget")
                        font.family: AccountStyle.title.name
                    }
                    
                    AccountCheckBox {
                        objectName: "budget"
                    }
                    
                    AccountLabel {
                        text: qsTr("Frequency")
                        font.family: AccountStyle.title.name
                    }
                    
                    AccountCheckBox {
                        objectName: "frequency"
                    }
                    
                    AccountLabel {
                        text: qsTr("CommonExpanse")
                        font.family: AccountStyle.title.name
                    }
                    
                    AccountCheckBox {
                        objectName: "common"
                    }
                    
                }
            }
            
            Component {
                id: db
                AccountComboBox {
                    textRole: "text"
                    valueRole: "value"
                    
                    onCurrentValueChanged: console.log(currentValue)
                    model: ListModel {
                        ListElement {
                            text: qsTr("Xml")
                            value: "ControllerXMLMulti"
                        }
                        
                        ListElement {
                            text: qsTr("Sql")
                            value: "ControllerDB"
                        }
                    }
                }
            }
            
            GroupBox {
                id: database
                width: parent.width
                label: AccountHeader {
                    text: qsTr("Database")
                    width:database.width
                    
                    height: root.height * 0.10
                }
                
                background: AccountBackground{invisible: true}
                
                GridLayout {
                    columns: 3
                    columnSpacing: root.width * 0.02
                    rowSpacing: root.height * 0.02

                    AccountLabel {
                        Layout.column: 0
                        Layout.row: 0
                        text: qsTr("Main database")
                        font.family: AccountStyle.title.name
                    }
                    
                    Loader {
                        Layout.column: 1
                        Layout.row: 0
                        objectName: "primary"
                        active: true
                        sourceComponent: db
                    }
                    
                    
                    
                    AccountCheckBox {
                        Layout.column: 2
                        Layout.row: 1
                        
                        id: secondadyEnable
                        objectName: "useBackup"
                        text: qsTr("Enable")
                        checked: true
                    }
                    
                    AccountLabel {
                        Layout.column: 0
                        Layout.row: 1
                        text: qsTr("Backup database")
                        font.family: AccountStyle.title.name
                    }
                    
                    Loader {
                        Layout.column: 1
                        Layout.row: 1
                        objectName: "backup"
                        sourceComponent: db
                        active: secondadyEnable.checked
                    }
                }
            }
        }
    }
}
