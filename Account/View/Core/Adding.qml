import QtQuick 2.11
import QtQml 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.13

import "../Style"
import "../Functionnal"

Popup {
    id: root

    function reset() {
        valueLabel.text = ""
        spinbox.value = 0
    }

    signal accept()

    property real v_val: spinbox.realValue
    property string v_title: valueLabel.text
    property string v_type: accountSelect.model.get(accountSelect.currentIndex).type
    property string v_date: dateLabel.text
    property string v_member: member.text
    property bool newAccount: false
    property bool common: false
    property var completionList: []
    property var openDate

    onClosed: {
        valueLabel.background.border.color = "#bdbdbd"
    }

    onOpened: {
        l_new.text = newAccount ? qsTr("Add new account"): qsTr("Add new entry")
        dateLabel.extern(new Date())
    }

    onOpenDateChanged: dateLabel.extern(openDate)

    leftPadding: width * 0.02
    rightPadding: width * 0.02
    topPadding: height * 0.05
    bottomPadding: height * 0.05

    background: AccountBackground{}


    contentItem: Rectangle {
        color: "transparent"

        GridLayout {
            id: grid

            rowSpacing: root.height * 0.02
            columnSpacing: root.width * 0.02

            anchors.fill: parent

            property double firstRow: root.height * 0.25
            property double secondRow: root.height * 0.38
            property double thirdRow: root.height * 0.23

            AccountLabel {
                id: labelAdd
                text: qsTr("Adding")
                font.family: AccountStyle.title.name
                font.pixelSize: AccountStyle.title.size

                Layout.preferredHeight: parent.firstRow
                Layout.preferredWidth: root.width * 0.15
                Layout.row: 0
                Layout.column: 0
                Layout.rowSpan: 1
                Layout.columnSpan: 1
            }

            CalendarButton {
                id: dateLabel

                Layout.preferredHeight: parent.firstRow
                Layout.preferredWidth: root.width * 0.25
                Layout.row: 0
                Layout.column: 1
                Layout.rowSpan: 1
                Layout.columnSpan: 1
            }

            AccountLabel{
                visible: common
                text: qsTr("Member")
                font.family: AccountStyle.title.name
                font.pixelSize: AccountStyle.title.size

                Layout.preferredHeight: parent.firstRow
                Layout.preferredWidth: root.width * 0.09
                Layout.row: 0
                Layout.column: 2
                Layout.rowSpan: 1
                Layout.columnSpan: 1
            }

            AccountTextInput {
                id: member
                objectName: "member"
                visible: common
                valid: (common && length > 0) || !common

                Layout.preferredHeight: parent.firstRow
                Layout.preferredWidth: root.width * 0.41
                Layout.row: 0
                Layout.column: 3
                Layout.rowSpan: 1
                Layout.columnSpan: 3

                ToolTip.text: qsTr("Fill a member name")

                onTextEdited: {
                    var s = -1
                    for(var i in root.completionList) {
                        if(root.completionList[i].indexOf(text) === 0)
                            s = i
                    }

                    compl = s !== -1 ? root.completionList[s] : ""
                }
            }

            AccountLabel {
                id: l_new
                text: qsTr("Add new \n entry")
                wrapMode: Text.WordWrap
                font.family: AccountStyle.title.name
                font.pixelSize: AccountStyle.title.size2

                Layout.preferredHeight: parent.secondRow
                Layout.preferredWidth: root.width * 0.15
                Layout.row: 1
                Layout.column: 0
                Layout.rowSpan: 1
                Layout.columnSpan: 1
            }

            AccountTextInput {
                id: valueLabel
                font.family: AccountStyle.core.name
                font.pixelSize: AccountStyle.core.size

                ToolTip.text: root.newAccount ? qsTr("Please enter an account name") : qsTr("Please complete the entry's title")
                ToolTip.visible: !valid || valueLabel.hovered

                valid: text.length !== 0

                Layout.preferredHeight: parent.secondRow
                Layout.preferredWidth: root.width * 0.25
                Layout.row: 1
                Layout.column: 1
                Layout.rowSpan: 1
                Layout.columnSpan: 1
            }

            AccountSpinbox {
                id: spinbox

                ToolTip.text: root.newAccount ? qsTr("Specify account opening value") : qsTr("Specify transaction's value")

                Layout.preferredHeight: parent.secondRow
                Layout.preferredWidth: root.width * 0.25
                Layout.row: 1
                Layout.column: 2
                Layout.rowSpan: 1
                Layout.columnSpan: 2
            }

            AccountComboBox {
                id: accountSelect
                objectName: "type"
                textRole: "name"
                model: CoreModel.typeModel

                Layout.preferredHeight: parent.secondRow
                Layout.preferredWidth: root.width * 0.25
                Layout.row: 1
                Layout.column: 4
                Layout.rowSpan: 1
                Layout.columnSpan: 2

                enabled: !root.newAccount

                ToolTip.text: qsTr("Specify income or outcome")
            }

            AccountButton {
                id: b_save
                text: qsTr("Save")

                ToolTip.text:  root.newAccount ? qsTr("Please enter an account name") : qsTr("Please complete the entry's title")
                ToolTip.visible: ttVisible

                property bool ttVisible: false
                onClicked: {
                    if(valueLabel.valid && member.valid) {
                        _main.adding()
                        reset()
                        close()
                    }
                    else {
                        ttVisible = true
                        valueLabel.background.border.color = "red"
                    }
                }

                Layout.preferredHeight: parent.thirdRow
                Layout.preferredWidth: root.width * 0.11
                Layout.row: 2
                Layout.column: 4
                Layout.rowSpan: 1
                Layout.columnSpan: 1

            }

            AccountButton {
                id:b_cancel
                text: qsTr("Cancel")

                onClicked: {
                    reset()
                    close()
                }

                Layout.preferredHeight: parent.thirdRow
                Layout.preferredWidth: root.width * 0.10
                Layout.row: 2
                Layout.column: 5
                Layout.rowSpan: 1
                Layout.columnSpan: 1
                ToolTip.timeout: 1
            }
        }
    }
}

