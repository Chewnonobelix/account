import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

Item {
    id: info
    objectName: "info"

    property int eid
    property string v_category
    property bool v_estimated

    RowLayout {
        Label {
            id: title
        }

        DoubleSpinBox {
            enabled: false
        }

        ComboBox {
            id: category
            objectName: "category"
            model: ["Transport", "Loyer", "Energie", "Telecom"]
            onCurrentTextChanged: {
                v_category = currentText
            }
        }

        CheckBox {
            id: estimated
            objectName: "estimated"
            text: qsTr("Estimated")
            onCheckedChanged: {
                v_estimated = checked
            }
        }
    }

    //Frequency
}
