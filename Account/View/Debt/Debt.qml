import QtQuick 2.15
import QtQuick.Layouts 1.15

import "../Style"
import "../Core"

Item {
    id: root

    Connections {
        target: _debt

        function onModelChanged(list, index) {
            debtView.model = list
            debtView.currentIndex = index
        }
    }

    Component.onCompleted: _debt.exec()
    GridLayout {
        anchors {
            fill: parent
            topMargin: root.height * 0.02
            bottomMargin: root.height * 0.02
            leftMargin: root.width * 0.02
            rightMargin: root.width * 0.02
        }

        columnSpacing: root.width * 0.02
        rowSpacing: root.height * 0.02

        ListView {
            id: debtView
            clip: true
            AccountBackground {
                anchors.fill: parent
                invisible: true
            }

            Layout.column: 0
            Layout.row: 0
            Layout.columnSpan: 2
            Layout.rowSpan: 9
            Layout.preferredHeight: root.height *0.88
            Layout.preferredWidth: root.width *0.20

            property var currentModel: null

            onCurrentIndexChanged: {
                currentModel = model[currentIndex]
            }

            onCurrentModelChanged: {
                if(currentModel) {
                    _debt.currentId = currentModel.id
                    name.text = currentModel.name
                    initial.entry = currentModel.initial
                    date.extern(currentModel.initial.date)
                    type.currentIndex = CoreModel.typeModel.find(currentModel.initial.type)
                    rate.value = currentModel.rate * 10000
                    time.value = currentModel.time * 100
                    frequencydebt.currentIndex = CoreModel.freqModel.findIndex(currentModel.freq)
                    entryList.model = currentModel.entries
                }
            }

            header: AccountHeader {
                height: debtView.height * .10
                width: debtView.width

                text: qsTr("Debt and loan")
            }

            delegate: Rectangle {
                Component.onCompleted: {
                }
                property var model: modelData
                height: debtView.height * .05
                width: debtView.width

                gradient: debtView.currentIndex === index ? AccountStyle.calSelect : AccountStyle.unselectView


                AccountLabel {
                    anchors.fill: parent
                    text: modelData.name === "" ? modelData.id : modelData.name
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked:{
                        debtView.currentIndex = debtView.currentIndex !== index ? index : -1
                    }
                }
            }
        }

        AccountButton {
            Layout.column: 0
            Layout.row: 9
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.06
            Layout.preferredWidth: root.width *0.09

            text: qsTr("Add")

            onClicked: _debt.addDebt()
        }

        AccountButton {
            Layout.column: 1
            Layout.row: 9
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.06
            Layout.preferredWidth: root.width *0.09
            Layout.alignment: Qt.AlignLeft
            text: qsTr("Remove")

            enabled: debtView.currentIndex != -1

            onClicked: _debt.onRemoved(debtView.currentModel.id)
        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 0
            Layout.columnSpan: 4
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.05
            Layout.preferredWidth: root.width *0.74

            text: qsTr("Name")
            visible: debtView.currentIndex != -1
        }

        AccountTextInput {
            id: name
            Layout.column: 2
            Layout.row: 1
            Layout.columnSpan: 4
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.05
            Layout.preferredWidth: root.width *0.74

            visible: debtView.currentIndex != -1

            onTextEdited:_debt.onNameChanged(debtView.currentModel.id, text)
        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 2
            Layout.columnSpan: 4
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.05
            Layout.preferredWidth: root.width *0.74

            text: qsTr("Reference")
            visible: debtView.currentIndex != -1
        }

        EntryEdit {
            id: initial
            Layout.column: 2
            Layout.row: 3
            Layout.columnSpan: 4
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.22
            Layout.preferredWidth: root.width *0.74

            onEntryChanged: {
            }
            visible: debtView.currentIndex != -1
            onValueChanged: {
                _debt.onInitialValueChanged(debtView.currentModel.id, value)
            }
            onCatChanged: _debt.onInitialValueChanged(debtView.currentModel.id, cat)
            onSupportChanged: _debt.onInitialSupportChanged(debtView.currentModel.id, supp)
            onAddNewCategory: _debt.onNewCategory(type.currentValue, cat)
        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 4
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.08

            text: qsTr("Date")
            visible: debtView.currentIndex != -1
        }

        CalendarButton {
            id: date
            Layout.column: 3
            Layout.row: 4
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.10

            visible: debtView.currentIndex != -1

            onTextChanged: if(down)  _debt.onInitialDateChanged(debtView.currentModel.id, Date.fromLocaleDateString(Qt.locale(),text, "dd-MM-yyyy"))
        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 5
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.08

            text: qsTr("Type")
            visible: debtView.currentIndex != -1
        }

        AccountComboBox {
            id: type
            Layout.column: 3
            Layout.row: 5
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.10

            visible: debtView.currentIndex != -1

            model: CoreModel.typeModel
            textRole: "name"
            valueRole: "type"

            onCurrentValueChanged: if(down) _debt.onInitialTypeChanged(debtView.currentModel.id, currentValue)
        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 6
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.08

            text: qsTr("Rate")
            visible: debtView.currentIndex != -1
        }

        AccountSpinbox {
            id: rate
            Layout.column: 3
            Layout.row: 6
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.10

            visible: debtView.currentIndex != -1

            onS_realVCalueChange: _debt.onRateChanged(debtView.currentModel.id, value)
        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 7
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.08

            text: qsTr("Time")
            visible: debtView.currentIndex != -1
        }

        AccountSpinbox {
            id: time
            Layout.column: 3
            Layout.row: 7
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.10
            stepSize: 100
            editable: false
            visible: debtView.currentIndex != -1

            onS_realVCalueChange: _debt.onTimeChanged(debtView.currentModel.id, value)
        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 8
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.08

            text: qsTr("Frequency")
            visible: debtView.currentIndex != -1
        }

        AccountComboBox {
            id: frequencydebt
            Layout.column: 3
            Layout.row: 8
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.10

            visible: debtView.currentIndex != -1
            model: CoreModel.freqModel
            textRole: "name"
            valueRole: "role"

            onCurrentValueChanged: if(debtView.currentModel && down) _debt.onFreqChanged(debtView.currentModel.id, currentValue)
        }

        AccountButton {
            Layout.column: 2
            Layout.row: 9
            Layout.columnSpan: 2
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.06
            Layout.preferredWidth: root.width *0.20

            text: qsTr("Generate")
            visible: debtView.currentIndex != -1
            enabled: debtView.currentModel ? debtView.currentModel.entries.length === 0 : true

            onClicked: _debt.generate(debtView.currentModel.id)
        }

        ListView {
            id: entryList
            Layout.column: 4
            Layout.row: 4
            Layout.columnSpan: 1
            Layout.rowSpan: 6
            Layout.preferredHeight: root.height *0.53
            Layout.preferredWidth: root.width *0.25

            visible: debtView.currentIndex != -1

            onCurrentIndexChanged: textEntry.model = model[currentIndex]
            delegate: AccountLabel {
                height: entryList.height * 0.05
                width: entryList.width
                text: modelData.title
                background: Rectangle {
                    gradient: entryList.currentIndex === index ? AccountStyle.calSelect : AccountStyle.unselectView
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: entryList.currentIndex = index
                }
            }
        }

        EntryText {
            id: textEntry
            Layout.column: 5
            Layout.row: 4
            Layout.columnSpan: 1
            Layout.rowSpan: 6
            Layout.preferredHeight: root.height *0.53
            Layout.preferredWidth: root.width *0.25

            visible: debtView.currentIndex != -1
        }
    }
}