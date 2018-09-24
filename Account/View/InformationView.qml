import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

Item {
    id: info
    objectName: "info"

    Item {
        id: entry
        objectName: "entry"
        property double value
        property int id
    }

    Item {
        id: infoModel
        objectName: "infoModel"
    }

//    property Information modelInf
//    property Entry modelEntry
    property int eid
    property string v_category
    property bool v_estimated

    RowLayout {
        id: row
        TextField {
            id: title
//            text: infoModel.title


        }

        DoubleSpinBox {
            enabled: false
            value: entry.value *100

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

//            checked: infoModel.estimated
        }
    }

    Label {
        anchors.top:row.bottom
        anchors.topMargin: 10
        text: "Coming Soon"
    }

    //Frequency
}
