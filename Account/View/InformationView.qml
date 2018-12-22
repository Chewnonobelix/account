import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.12

Item {
    id: info
    objectName: "info"

    AccountStyle {
        id: pageStyle
    }
    property int maximum: Screen.width * .55 - 10
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
        horizontalAlignment: Qt.AlignHCenter
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
        horizontalAlignment: Qt.AlignHCenter
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
        horizontalAlignment: Qt.AlignHCenter
        background: Rectangle {
            gradient: pageStyle.goldHeader
        }
    }
    TextField {
        id: title
        width: maximum / 3
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
        width: maximum / 3

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
        width: maximum / 3

        anchors.top: categoryLabel.bottom
        anchors.topMargin: 5
        anchors.left: spinbox.right
        editable: currentText === ""
        model: [""]


        signal s_addCategory(string cat)
        signal s_currentTextChanged(string cat)

        contentItem: StackLayout {
            currentIndex: category.editable ? 1 : 0
            Label {
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                text: category.currentText
            }

            TextField {
                id: editCombo

                Keys.onPressed: {
                    if((event.key === Qt.Key_Enter || event.key === Qt.Key_Return)  && category.currentIndex === category.model.length-1) {
                        category.addCat(text)
                        clear()
                    }
                }
            }
        }

        onContentItemChanged: console.log("content", contentItem, editable, ec)
        onCurrentTextChanged: {
            s_currentTextChanged(currentText)
        }

        MouseArea {
            anchors.fill: parent
            z: -1
            cursorShape: Qt.PointingHandCursor
            acceptedButtons: Qt.NoButton
        }

        function addCat(newCat) {
            var tmp = model
            tmp.pop()
            tmp.push(newCat)
            tmp.push("")
            model = tmp
            currentIndex = tmp.length-2
            s_addCategory(newCat)
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

        background: Rectangle {
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

                    ToolTip.visible: (index === (category.count - 1)) && hovered
                    ToolTip.text: qsTr("Add a new category")
                    ToolTip.delay: 500

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
