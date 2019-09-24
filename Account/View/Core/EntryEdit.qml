import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.12
import "../Style" as S

Item {
    id: info
    property bool opening: false
    S.AccountStyle {
        id: pageStyle
    }
    property int maximum: Screen.width * .55 - 10
    width: spinbox.width + category.width + title.width
    
    property var entry
    property var infoModel: entry.info
    

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
                if(!info.opening)
                    info.s_valueChanged(spinbox.realValue)

            }
        }

        onRealValueChanged: {
            timer.restart()
        }
    }

    CategoryItem {
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

    }
    
}
