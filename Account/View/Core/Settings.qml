import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "../Style"

Dialog {
    AccountStyle {
        id: pageStyle
    }
    
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
            background: Rectangle {gradient: pageStyle.goldButton}
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
                label: Loader {
                    sourceComponent: pageStyle.compLabel
                    
                    onLoaded: {
                        item.text = qsTr("General")
                        item.width = Qt.binding(function() {return general.width})
                        item.height = Qt.binding(function() {return root.height * 0.10})
                    }
                }
                
                background: Rectangle {
                    color: "transparent"
                    border.color: "gold"
                }

                GridLayout {
                    columns: 2
                    
                    Loader {
                        sourceComponent: pageStyle.compLabel
                        onLoaded: item.text = qsTr("Language")
                    }
                    
                    ComboBox {
                        objectName: "language"
                    }
                }
            }
            
            
            GroupBox {
                id: features
                width: parent.width
                label: Loader {
                    sourceComponent: pageStyle.compLabel
                    
                    onLoaded: {
                        item.text = qsTr("Features")
                        item.width = Qt.binding(function() {return features.width})
                        item.height = Qt.binding(function() {return root.height * 0.10})
                    }
                }
                
                background: Rectangle {
                    color: "transparent"
                    border.color: "gold"
                }

                GridLayout {
                    columns: 2
                    Loader {
                        sourceComponent: pageStyle.compLabel
                        onLoaded: item.text = qsTr("Budget")
                    }
                    
                    CheckBox {
                        objectName: "budget"
                    }
                    
                    Loader {
                        sourceComponent: pageStyle.compLabel
                        onLoaded: item.text = qsTr("Frequency")
                    }
                    
                    CheckBox {
                        objectName: "frequency"
                    }
                    
                    Loader {
                        sourceComponent: pageStyle.compLabel
                        onLoaded: item.text = qsTr("CommonExpanse")
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
                label: Loader {
                    onLoaded: {
                        item.text = qsTr("Database")
                        item.width = Qt.binding(function() {return database.width})
                        item.height = Qt.binding(function() {return root.height * 0.10})
                    }
                    
                    sourceComponent: pageStyle.compLabel
                }
                 
                background: Rectangle {
                    color: "transparent"
                    border.color: "gold"
                }
                
                GridLayout {
                    columns: 3
                    
                    Loader {
                        Layout.column: 1
                        Layout.row: 0
                        sourceComponent: pageStyle.compLabel
                        onLoaded: item.text = qsTr("Main database")
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
                    
                    Loader {
                        Layout.column: 1
                        Layout.row: 1
                        
                        sourceComponent: pageStyle.compLabel
                        onLoaded: item.text = qsTr("Backup database")
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

