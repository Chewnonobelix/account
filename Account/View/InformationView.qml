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
        property bool estimated
        property string title
        property string type
    }

    signal s_titleChanged(string title)
    signal s_estimatedChanged(bool title)

    TextField {
        id: title

        text: infoModel.title

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
        editable: currentText === ""
        model: ["Transport", "Loyer", "Energie", "Telecom", ""]


        Keys.onPressed: {
            if(event.key === Qt.Key_Enter && currentIndex === model.length-1) {
                var tmp = model
                tmp.pop()
                tmp.push(editText)
                tmp.push("")
                model = tmp
                currentIndex = tmp.length-2
            }
        }

        function setting(type) {
            var index = find(type)
            if(index >= 0) {
                currentIndex = index
            }
            else {
                currentIndex = 0
            }
        }

        Rectangle {
            anchors.fill: parent
            gradient: pageStyle.goldButton
        }

        Component.onCompleted: {
            console.log(model.length)
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

                MouseArea {
                    property string cTxt: modelData
                    anchors.fill: parent
                    cursorShape: Qt.WhatsThisCursor

                    onClicked: {
                        if(mouse.button === Qt.LeftButton) {
                            var index = category.find(cTxt)
                            category.currentIndex = index
                            category.popup.close()
                        }
                    }
                }

            }
        }
    }

    CheckBox {
        id: estimated
        objectName: "estimated"
        anchors.left: category.right

        text: qsTr("Estimated")

        checked: infoModel.estimated

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
