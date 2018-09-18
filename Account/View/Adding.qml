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
    property string v_date

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
            }

            DoubleSpinBox {
                id: spinbox
            }

            ComboBox {
                id: type
                objectName: "type"
                model: ["Income", "Outcome"]
            }
        }

        RowLayout {
            Button {
                id: b_save
                text: qsTr("Save")

                onClicked: {
                    console.log(addingid.v_date + " Accept")
                    addingid.accept()
                    reset()
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

