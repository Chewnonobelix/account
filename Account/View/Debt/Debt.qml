import QtQuick 2.15
import QtQuick.Layouts 1.15

import "../Style"
import "../Core"

Item {
    id: root

    Connections {
        target: _debt

        function onModelChanged(list) {
            debtView.model = list
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
                name.text = currentModel.name
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
                        debtView.currentIndex = index
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

            text: qsTr("Remove")

            enabled: debtView.currentIndex != -1
        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 0
            Layout.columnSpan: 4
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.05
            Layout.preferredWidth: root.width *0.74

            text: qsTr("Name")
            enabled: debtView.currentIndex != -1
        }

        AccountTextInput {
            id: name
            Layout.column: 2
            Layout.row: 1
            Layout.columnSpan: 4
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.05
            Layout.preferredWidth: root.width *0.74

            enabled: debtView.currentIndex != -1

            onTextEdited: _debt.onNameChanged(debtView.currentModel.id, text)
        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 2
            Layout.columnSpan: 4
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.05
            Layout.preferredWidth: root.width *0.74

            text: qsTr("Reference")
            enabled: debtView.currentIndex != -1
        }

        EntryEdit {
            Layout.column: 2
            Layout.row: 3
            Layout.columnSpan: 4
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.22
            Layout.preferredWidth: root.width *0.74

            enabled: debtView.currentIndex != -1
        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 4
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.08

            text: qsTr("Date")
            enabled: debtView.currentIndex != -1
        }

        CalendarButton {
            Layout.column: 3
            Layout.row: 4
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.10

            enabled: debtView.currentIndex != -1
        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 5
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.08

            text: qsTr("Type")
            enabled: debtView.currentIndex != -1
        }

        AccountComboBox {
            Layout.column: 3
            Layout.row: 5
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.10

            enabled: debtView.currentIndex != -1
        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 6
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.08

            text: qsTr("Rate")
            enabled: debtView.currentIndex != -1
        }

        AccountSpinbox {
            Layout.column: 3
            Layout.row: 6
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.10

            enabled: debtView.currentIndex != -1
        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 7
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.08

            text: qsTr("Time")
            enabled: debtView.currentIndex != -1
        }

        AccountSpinbox {
            Layout.column: 3
            Layout.row: 7
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.10

            enabled: debtView.currentIndex != -1
        }

        AccountHeader {
            Layout.column: 2
            Layout.row: 8
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.08

            text: qsTr("Frequency")
            enabled: debtView.currentIndex != -1
        }

        AccountComboBox {
            Layout.column: 3
            Layout.row: 8
            Layout.columnSpan: 1
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.07
            Layout.preferredWidth: root.width *0.10

            enabled: debtView.currentIndex != -1
        }

        AccountButton {
            Layout.column: 2
            Layout.row: 9
            Layout.columnSpan: 2
            Layout.rowSpan: 1
            Layout.preferredHeight: root.height *0.06
            Layout.preferredWidth: root.width *0.20

            text: qsTr("Generate")
            enabled: debtView.currentIndex != -1
        }

        ListView {
            Layout.column: 4
            Layout.row: 4
            Layout.columnSpan: 1
            Layout.rowSpan: 6
            Layout.preferredHeight: root.height *0.53
            Layout.preferredWidth: root.width *0.25

            enabled: debtView.currentIndex != -1
        }

        EntryText {
            Layout.column: 5
            Layout.row: 4
            Layout.columnSpan: 1
            Layout.rowSpan: 6
            Layout.preferredHeight: root.height *0.53
            Layout.preferredWidth: root.width *0.25

            enabled: debtView.currentIndex != -1
        }
    }
}
