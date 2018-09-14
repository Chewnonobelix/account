import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import Account 1.0

Item {
    id: info
    objectName: "info"

//    property Information modelInf
    property Entry modelEntry
    property int eid
    property string v_category
    property bool v_estimated

    RowLayout {
        Label {
            id: title
//            text: modelInf.title


        }

        DoubleSpinBox {
            enabled: false
            value: modelEntry.value * 100

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

//            checked: modelInf.estimated
        }
    }

    //Frequency
}
