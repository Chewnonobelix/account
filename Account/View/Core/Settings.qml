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
    ScrollView {
        anchors.fill: parent
        clip: true
        ColumnLayout {
            clip: true
            anchors.fill: parent

            GroupBox {
                title: qsTr("General")

                GridLayout {
                    columns: 2

                    Label {
                        text: qsTr("Language")
                    }

                    ComboBox {
                        objectName: "language"
                        model: ["English", "Francais"]
                    }
                }
            }


            GroupBox {
                title: qsTr("Features")

                GridLayout {
                    columns: 2
                    Label {
                        text: qsTr("Budget")
                    }

                    CheckBox {
                        objectName: "budget"
                    }

                    Label {
                        text: qsTr("Frequency")
                    }

                    CheckBox {
                        objectName: "frequency"
                    }

                    Label {
                        text: qsTr("Common Expanse")
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
                title: qsTr("Database")

                GridLayout {
                    columns: 3

                    Label {
                        text: qsTr("Main database")
                    }

                    Loader {
                        objectName: "primary"
                        active: true
                        sourceComponent: db
                    }

                    Frame {
                        visible: false
                    }

                    CheckBox {
                        id: secondadyEnable
                        objectName: "useBackup"
                        text: qsTr("Enable")
                        checked: true
                    }

                    Label {
                        text: qsTr("Backup database")
                    }

                    Loader {
                        objectName: "backup"
                        sourceComponent: db
                        active: secondadyEnable.checked
                    }

                }
            }
        }
    }
}
