import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.12
import Account 1.0
import "../Style"
import "../Functionnal"

AccountBackground {
    id: root
    invisible: true
    property bool opening: false
    property var entry: null
    property var catModel: []
    borderEnabled: false

    signal s_titleChanged(string title)
    signal s_estimatedChanged(bool title)
    signal s_valueChanged(real value)
    signal s_catChanged(string cat)
    signal s_supportChanged(int supp)
    signal addNewCategory(string cat)

    function changeDirection() {
        grid.flow = grid.flow === GridLayout.LeftToRight ? GridLayout.TopToBottom : GridLayout.LeftToRight

    }

    onEntryChanged: {
        title.text = Qt.binding(function() {return entry ? entry.info.title : ""})
        spinbox.value = Qt.binding(function() {return entry ? entry.value * 100 : 0})
        category.model = Qt.binding(function() {return catModel})
        category.currentIndex = Qt.binding(function() {return entry ? category.setting(entry.info.category) : category.model.length - 1})
        support.currentIndex = Qt.binding(function () {return support.model.findIndex(entry ? entry.support : Account.CB) })
    }

    GridLayout {
        id: grid
        rows: 4
        columns: 4
        anchors.fill: parent
        rowSpacing: height * 0.01
        columnSpacing: width * 0.01


        onEnabledChanged: {
            titleLabel.enabled = true
            categoryLabel.enabled = true
            valueLabel.enabled = true
        }

        property var tWidth: flow === GridLayout.LeftToRight ? 0.25 : 1
        property var tHeight: flow === GridLayout.LeftToRight ? 1 : 0.25

        Column {
            Layout.preferredWidth: grid.width * grid.tWidth
            Layout.preferredHeight: grid.height * grid.tHeight
            spacing: grid.height * 0.02

            AccountHeader {
                id: titleLabel
                text: qsTr("Title")

                width: parent.width
                height: parent.height * 0.39
            }

            AccountTextInput {
                id: title
                enabled: !opening
                width: parent.width
                height: parent.height * 0.59
                text: ""

                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                onEditingFinished: {
                    s_titleChanged(text)
                    opening = false
                    enabled = true
                }

                ToolTip.text: qsTr("Change transaction's title")

                onTextEdited: {
                    enabled = false
                    opening = true
                }
            }
        }

        Column {
            spacing: parent.height * 0.02
            Layout.preferredWidth: grid.width * grid.tWidth
            Layout.preferredHeight: grid.height *grid.tHeight

            AccountHeader {
                id: valueLabel
                text: qsTr("Value")

                height: parent.height * .39
                width: parent.width
            }

            AccountSpinbox {
                id: spinbox

                height: parent.height * 0.59
                width: parent.width
                enabled: !opening

                value: 0

                ToolTip.text: qsTr("Change transaction's value")

                property date s_date

                onS_realVCalueChange: s_valueChanged(realValue)

            }

        }

        Column {
            spacing: grid.height * 0.02
            Layout.preferredWidth: grid.width * grid.tWidth
            Layout.preferredHeight: grid.height * grid.tHeight

            AccountHeader {
                id: categoryLabel
                text: qsTr("Category")

                height: parent.height * 0.39
                width: parent.width
            }

            CategoryItem {
                id: category
                objectName: "category"
                enabled: !grid.opening

                height: parent.height * 0.59
                width: parent.width

                editable: currentText === ""

                onS_addCategory: root.addNewCategory(cat)

                onS_currentTextChanged: {
                    if(currentText !== "")
                        s_catChanged(currentText)
                }
            }
        }

        Column {
            spacing: grid.height * 0.02
            Layout.preferredWidth: grid.width * grid.tWidth
            Layout.preferredHeight: grid.height * grid.tHeight

            AccountHeader {
                id: supportLabel
                text: qsTr("Support")

                height: parent.height * 0.39
                width: parent.width
            }

            AccountComboBox {
                id: support
                objectName: "support"
                signal s_supportChanged(int support)
                enabled: !grid.opening
                model: CoreModel.entryTypeModel
                height: parent.height * 0.59
                width: parent.width
                textRole: "name"
                valueRole: "role"
                
                ToolTip.text: qsTr("Select transaction's support type")
                
                onCurrentValueChanged: {
                    if(down) {
                        root.s_supportChanged(currentValue)
                        s_supportChanged(currentValue)
                    }
                }
            }
        }
    }
}
