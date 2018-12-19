import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

Item {
    id: info
    objectName: "info"

    AccountStyle {
        id: pageStyle
    }

    width: spinbox.width + category.width + title.width
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


    onEnabledChanged: {
        titleLabel.enabled = true
        categoryLabel.enabled = true
        valueLabel.enabled = true
    }

    Label {
        id: titleLabel
        text: qsTr("Title")
        width: title.width
        font.pixelSize: pageStyle.title.size
        font.family: pageStyle.title.name
        background: Rectangle {
            gradient: pageStyle.goldHeader
        }
    }

    Label {
        id: valueLabel
        text: qsTr("Value")
        width: spinbox.width
        anchors.left: titleLabel.right
        font.pixelSize: pageStyle.title.size
        font.family: pageStyle.title.name
        background: Rectangle {
            gradient: pageStyle.goldHeader
        }
    }
    Label {
        id: categoryLabel
        text: qsTr("Category")
        width: category.width
        anchors.left: valueLabel.right
        font.pixelSize: pageStyle.title.size
        font.family: pageStyle.title.name
        background: Rectangle {
            gradient: pageStyle.goldHeader
        }
    }
    TextField {
        id: title

        anchors.top:titleLabel.bottom
        anchors.topMargin: 5
        text: infoModel.title
        font.family: pageStyle.core.name
        font.pixelSize: pageStyle.core.size
        onEditingFinished: {
            s_titleChanged(text)
        }

    }

    DoubleSpinBox {
        id: spinbox
        value: entry.value*100
        anchors.top: valueLabel.bottom
        anchors.topMargin: 5
        anchors.left: title.right
        enabled: false
        font.family:  pageStyle.core.name
        font.pixelSize: pageStyle.core.size

    }

    ComboBox {
        id: category
        objectName: "category"
        anchors.top: categoryLabel.bottom
        anchors.topMargin: 5
        anchors.left: spinbox.right
        editable: currentText === ""
        model: [""]
        font.family: pageStyle.core.name
        font.pixelSize: pageStyle.core.size

        signal s_addCategory(string cat)
        signal s_currentTextChanged(string cat)

        onCurrentTextChanged: {
            s_currentTextChanged(currentText)
        }

        MouseArea {
            anchors.fill: parent
            z: -1
            cursorShape: Qt.PointingHandCursor
            acceptedButtons: Qt.NoButton
        }

        Keys.onPressed: {
            if((event.key === Qt.Key_Enter || event.key === Qt.Key_Return)  && currentIndex === model.length-1) {
                var tmp = model
                tmp.pop()
                tmp.push(editText)
                tmp.push("")
                model = tmp
                currentIndex = tmp.length-2
                s_addCategory(editText)
            }
        }

        function setting(type) {
            var index = find(type)

            if(index >= 0) {
                currentIndex = index
            }
            else {
                currentIndex = model.length-1
            }
        }

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
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                    anchors.centerIn: parent
                }

                MouseArea {
                    property string cTxt: modelData
                    anchors.fill: parent
                    cursorShape: index === (category.count - 1)  ? Qt.WhatsThisCursor : Qt.PointingHandCursor

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


    Label {
        anchors.top:title.bottom
        anchors.topMargin: 10
        text: "Coming Soon"
    }
    //Frequency
}
