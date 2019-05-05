import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.12

Item {
    id: info
    objectName: "info"

    property bool opening: false
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

        onTypeChanged: {
            category.setting(type)
        }
    }

    signal s_titleChanged(string title)
    signal s_estimatedChanged(bool title)
    signal s_valueChanged(real value)

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
            border.color: "darkgoldenrod"
        }
    }

    Label {
        id: valueLabel
        text: qsTr("Value")
        width: spinbox.width +10
        anchors.left: titleLabel.right
        font.pixelSize: pageStyle.title.size
        font.family: pageStyle.title.name
        horizontalAlignment: Qt.AlignHCenter
        background: Rectangle {
            gradient: pageStyle.goldHeader
            border.color: "darkgoldenrod"
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
            border.color: "darkgoldenrod"
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

        ToolTip.text: qsTr("Change transaction's title")
        ToolTip.visible: hovered
        ToolTip.delay: 500
    }

    DoubleSpinBox {
        id: spinbox
        width: maximum / 3

        value: entry.value*100
        anchors.top: valueLabel.bottom
        anchors.topMargin: 5
        anchors.left: title.right
        anchors.leftMargin: 5
//        anchors.rightMargin: 5
        font.family:  pageStyle.core.name
        font.pixelSize: pageStyle.core.size

        ToolTip.text: qsTr("Change transaction's value")
        ToolTip.visible: hovered
        ToolTip.delay: 500

        property date s_date
        Timer {
            id: timer
            repeat: false

            onTriggered: {
                if(!parent.opening)
                    info.s_valueChanged(parent.realValue)

            }
        }

        onRealValueChanged: {
            timer.restart()
        }
    }

    ComboBox {
        id: category
        objectName: "category"
        width: maximum / 3
        height: spinbox.height

        anchors.top: categoryLabel.bottom
        anchors.topMargin: 5
        anchors.left: spinbox.right
        anchors.leftMargin: 5
        editable: currentText === ""
        model: [""]

        ToolTip.text: qsTr("Set transaction's category")
        ToolTip.visible: hovered
        ToolTip.delay: 500

        onAccepted: {
            s_addCategory(editText)
        }
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
        text: qsTr("Coming Soon")
    }
    //Frequency
}
