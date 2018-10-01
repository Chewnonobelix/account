import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

Item {
    id: info
    objectName: "info"

    AccountStyle {
        id: pageStyle
    }

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

    signal s_titleChanged(string title)
    signal s_estimatedChanged(bool title)

    TextField {
        id: title

        onEditingFinished: {

            s_titleChanged(text)
        }

    }

    DoubleSpinBox {
        id: spinbox
        value: entry.value*100
        anchors.left: title.right
        enabled: false
    }

    ComboBox {
        id: category
        objectName: "category"
        anchors.left: spinbox.right

        model: ["Transport", "Loyer", "Energie", "Telecom"]

        Rectangle {
            anchors.fill: parent
            gradient: pageStyle.goldButton
        }

        delegate: ItemDelegate {
            width: category.width
            contentItem: Rectangle  {
                gradient: pageStyle.goldButton
                anchors.fill: parent
                Label {
                    color: "black"
                    text: modelData
                    anchors.centerIn: parent
                }
            }
        }
    }

    CheckBox {
        id: estimated
        objectName: "estimated"
        anchors.left: category.right

        text: qsTr("Estimated")

        onCheckStateChanged: {
            s_estimatedChanged(checked)
        }
    }

    Label {
        anchors.top:title.bottom
        anchors.topMargin: 10
        text: "Coming Soon"
    }
    //Frequency
}
