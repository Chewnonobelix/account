import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4

Page {

    implicitWidth: parent.width
    implicitHeight: parent.height

    Calendar {
        id: cal
        weekNumbersVisible: true
        implicitHeight: parent.height /2
        implicitWidth: parent.width * 0.2

        onClicked: {
        }
    }

   /* Popup {
        id: adding
        closePolicy: Popup.NoAutoClose

        function reset() {
            valueLabel.text = ""
            spinbox.value = 0
        }

        ColumnLayout {
            Label {
                id: labelAdd
                text: qsTr("Adding")
            }


            RowLayout {
                Label {
                    id: l_new
                    text: qsTr("Add new entry")
                }

                TextField {
                    id: valueLabel
                }

                SpinBox {
                    id: spinbox
                    from: 0
                    value: 110
                    to: 100 * 100
                    stepSize: 10
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100

                    validator: DoubleValidator {
                        bottom: Math.min(spinbox.from, spinbox.to)
                        top:  Math.max(spinbox.from, spinbox.to)
                    }

                    textFromValue: function(value, locale) {
                        return Number(value / 100).toLocaleString(locale, 'f', spinbox.decimals)
                    }

                    valueFromText: function(text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }
                }
            }

            RowLayout {
                Button {
                    id: b_save
                    text: qsTr("Save")

                    onClicked: {
                        tableModel.insert(tableModel.count, {"datec": cal.selectedDate.toLocaleDateString(), "value":spinbox.realValue, "labelc": valueLabel.text, "total":0})

                        /*datec: "2018-11-6"
                            value: 25.3
                            labelc: "X text"
                            total: 65.6*/

 /*                       adding.reset()
                        adding.close()


                    }
                }

                Button {
                    id:b_cancel
                    text: qsTr("Cancel")

                    onClicked: {
                        adding.reset()
                        adding.close()
                    }
                }
            }
        }
    }*/

    Item {
        anchors.top: cal.bottom
        width: cal.width

        Button {
            id: add
            flat: false
            text: qsTr("Add")
            width: parent.width * .45
            anchors.top: parent.top
            anchors.left: parent.left
            focus: true
           /* onClicked: {
                adding.x = pressX + x + parent.x
                adding.y = pressY + y + parent.y
                adding.open()
            }*/

            onClicked: mainWindow.adding()
        }


        Button {
            id: remove
            text: qsTr("Remove")
            width: parent.width * .45
            anchors.left: add.right
            anchors.leftMargin: (parent.width * .1)

            onClicked: {
                mainWindow.remove(view.currentRow)
            }

        }

        Button {
            id: edit
            text: qsTr("Edit")
            width: parent.width
            anchors.top: add.bottom
            anchors.topMargin: 10
            property int index

            onClicked: {
                console.log(index)
                mainWindow.edit(index)
            }

        }
    }

    ListModel {
        id: tableModel
        ListElement {
            datec: "2018-11-6"
            value: 25.3
            labelc: "X text"
            total: 65.6
        }

        ListElement {
            datec: "2018-5-6"
            value: 465
            labelc: "y text"
            total: 45.6
        }
    }

    TableView {
        anchors.left: cal.right
        width: parent.width - cal.width
        height: parent.height
        id: view
        TableViewColumn {
            role: "datec"
            title: qsTr("Date")
            width: 100
        }

        TableViewColumn {
            role: "value"
            title: qsTr("Value")
            width: 100
        }

        TableViewColumn {
            role: "labelc"
            title: qsTr("Label")
            width: 100
        }

        TableViewColumn {
            role: "total"
            title: qsTr("Total")
            width: 100
        }

        onClicked: {
            edit.index = row
        }

        onDoubleClicked: {
            console.log(model.get(row).labelc)
            clicked(row)
            edit.clicked()
        }

        model: tableModel
    }
}

