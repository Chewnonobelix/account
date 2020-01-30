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
    
    background: Rectangle {
        gradient: pageStyle.backgroundGradient
    }
    
    footer: DialogButtonBox {
        alignment: Qt.AlignRight
        
        delegate: Button {
            background: Rectangle {gradient: AccountStyle.goldButton}
        }
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
                label: AccountLabel {
                    text: qsTr("General")
                    width: general.width
                    height: root.height * 0.10
                }
                
                background: Rectangle {
                    color: "transparent"
                    border.color: "gold"
                }
                
                GridLayout {
                    columns: 2
                    
                    AccountLabel {
                        text: qsTr("Language")
                    }
                    
                    ComboBox {
                        objectName: "language"
                    }
                }
            }
            
            GroupBox {
                id: features
                width: parent.width
                label: AccountLabel {                    
                    text: qsTr("Features")
                    width: features.width
                    height: root.height * 0.10
                }
                
                background: Rectangle {
                    color: "transparent"
                    border.color: "gold"
                }
                
                GridLayout {
                    columns: 2
                    AccountLabel {
                        text: qsTr("Budget")
                    }
                    
                    CheckBox {
                        objectName: "budget"
                    }
                    
                    AccountLabel {
                        text: qsTr("Frequency")
                    }
                    
                    CheckBox {
                        objectName: "frequency"
                    }
                    
                    AccountLabel {
                        text: qsTr("CommonExpanse")
                    }
                    
                    CheckBox {
                        objectName: "common"
                    }
                    
                }
            }
            
            Component {
                id: db
                ComboBox {
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
                label: AccountLabel {
                    text: qsTr("Database")
                    width:database.width
                    
                    height: root.height * 0.10
                }
                
                background: Rectangle {
                    color: "transparent"
                    border.color: "gold"
                }
                
                GridLayout {
                    columns: 3
                    
                    AccountLabel {
                        Layout.column: 1
                        Layout.row: 0
                        text: qsTr("Main database")
                    }
                    
                    Loader {
                        Layout.column: 2
                        Layout.row: 0
                        objectName: "primary"
                        active: true
                        sourceComponent: db
                    }
                    
                    
                    
                    CheckBox {
                        Layout.column: 0
                        Layout.row: 1
                        
                        id: secondadyEnable
                        objectName: "useBackup"
                        text: qsTr("Enable")
                        checked: true
                    }
                    
                    AccountLabel {
                        Layout.column: 1
                        Layout.row: 1
                        text: qsTr("Backup database")
                    }
                    
                    Loader {
                        Layout.column: 2
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
