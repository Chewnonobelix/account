import QtQuick 2.11
import QtQuick.Controls 2.4

Popup {
    implicitHeight: parent.height
    implicitWidth: parent.width

    id: checker

    ListModel {
        signal validate()

        id: checkerModel
        objectName: "checkerModel"
        function fAdd(i) {
            append(i)
        }

        function idE(i) {
            return get(i).id
        }

        function isChecked(i) {
            return get(i).isChecked
        }
    }

    ListView {
        id: listChecker
        model: checkerModel
        width: parent.width
        height: parent.height * .94
        delegate: Row {
            CheckBox {
                checked: isChecked

                onCheckedChanged: {
                    isChecked = checked
                }
            }
            Text {
                text: label + " " + edate + " " + value + "€"
            }
        }
    }

    Button {
        text: qsTr("Save")
        anchors.top: listChecker.bottom
        anchors.topMargin: 10
        height: parent.height * .05
        onClicked: {
            checkerModel.validate()
        }
    }
}
