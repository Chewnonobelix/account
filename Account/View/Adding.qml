import QtQuick 2.11
import QtQml 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4

Popup {
    closePolicy: Popup.NoAutoClose

    function reset() {
        valueLabel.text = ""
        spinbox.value = 0
    }

    signal accept()

    property real v_val: spinbox.realValue
    property string v_title: valueLabel.text
    property string v_type: type.currentText
    property date v_date

    ColumnLayout {
        Label {
            id: labelAdd
            text: qsTr("Adding " + v_date)
        }



        RowLayout {
            Label {
                id: l_new
                text: qsTr("Add new entry")
            }

            TextField {
                id: valueLabel
//                onTextChanged: {
//                    v_title = text
//                }
            }

            DoubleSpinBox {
                id: spinbox
//                onValueChanged: {
//                    v_val = realValue
//                }
            }

            ComboBox {
                id: type
                objectName: "type"
                model: ["Income", "Outcome"]
//                onCurrentTextChanged: {
//                    v_type = currentText
//                }
            }
        }

        RowLayout {
            Button {
                id: b_save
                text: qsTr("Save")

                onClicked: {
                    //tableModel.insert(tableModel.count, {"datec": cal.selectedDate.toLocaleDateString(), "value":spinbox.realValue, "labelc": valueLabel.text, "total":0})

                    /*datec: "2018-11-6"
                                value: 25.3
                                labelc: "X text"
                                total: 65.6*/

                    addingid.accept()
                    close()
                }
            }

            Button {
                id:b_cancel
                text: qsTr("Cancel")

                onClicked: {
                    reset()
                    close()
                }
            }
        }
    }
}

