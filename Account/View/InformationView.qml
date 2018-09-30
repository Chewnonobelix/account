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

    property int eid
    property string v_category
    property bool v_estimated
//    clip:  true
        //        contentItem: row
        //        contentWidth: row.width
        //        contentHeight: row.height
        //        hoverEnabled: true
        ////        anchors.fill: parent

        //        Component.onCompleted: {
        //            console.log("Bordem de couille de boeuf")
        //            console.log(height)
        //            console.log(width)
        //        }
        //    }

//        RowLayout {
//            id: row
//            height: parent.height
//            width: parent.width
//                    clip: true
            TextField {
                id: title
            }

            DoubleSpinBox {
                id: spinbox
                value: entry.value*100
                anchors.left: title.right
                enabled: false
//                clip: true
            }

            ComboBox {
                id: category
                objectName: "category"
                anchors.left: spinbox.right

                model: ["Transport", "Loyer", "Energie", "Telecom"]
            }

            CheckBox {
                id: estimated
                objectName: "estimated"
                anchors.left: category.right

                text: qsTr("Estimated")
            }
//        }

        Label {
            anchors.top:title.bottom
            anchors.topMargin: 10
            text: "Coming Soon"
        }
    //Frequency
}
