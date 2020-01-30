import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "../Style"
import "../Functionnal"

Window {
    height: 500
    width: height * 9 / 16

    CoreModel {
        id: cModels
    }

    flags: Qt.Popup

    onActiveChanged: if(!active) close()
    id: root
    property var entry: Object()

    property var incomeCats: []
    property var outcomeCats: []

    AccountBackground {
        anchors.fill: parent
    }

    ScrollView {
        anchors.fill: parent
        anchors.leftMargin: root.width *.02
        anchors.rightMargin: root.width *.02
        anchors.topMargin: root.height *.02
        anchors.bottomMargin: root.height *.02

        GridLayout {
            columns: 2
            anchors.fill: parent

            AccountHeader {
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .96
                Layout.columnSpan: 2
                text: qsTr("Quick adding")
            }

            ComboBox {
                model: cModels.typeModel
                id: typeComboQuick
                objectName: "type"
                valueRole: "type"
                textRole: "name"

                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47

                font.family: AccountStyle.core.name
                font.pixelSize: AccountStyle.core.size

                ToolTip.text: qsTr("Specify income or outcome")
                ToolTip.visible: hovered
                ToolTip.delay: 500

                background:  Rectangle {
                    gradient: AccountStyle.goldButton
                    anchors.fill: parent
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    acceptedButtons: Qt.NoButton
                }
                delegate: ItemDelegate {
                    width: typeComboQuick.width

                    contentItem: Rectangle {
                        gradient: AccountStyle.goldButton
                        anchors.fill: parent
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            acceptedButtons: Qt.NoButton
                        }
                        AccountLabel {
                            color: "black"
                            text: name
                            anchors.centerIn: parent
                        }
                    }

                    highlighted: typeComboQuick.highlightedIndex === index
                }
            }

            AccountHeader {
                text: qsTr("Type")
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47
            }

            CalendarButton {
                id: dateButton
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47
            }

            AccountHeader {
                text: qsTr("Date")
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47
            }

            TextField {
                id: title
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47
            }

            AccountHeader {
                text: qsTr("Title")
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47
            }

            DoubleSpinBox {
                id: valueSpin
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47
            }

            AccountHeader {
                text: qsTr("Value")
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47
            }

            CategoryItem{
                id: catComboQuick
                objectName: "cat"
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47
                Component.onCompleted: model = Qt.binding(function() {return typeComboQuick.currentValue === "income" ? root.incomeCats : root.outcomeCats})
            }

            AccountHeader {
                text: qsTr("Category")
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47
            }

            AccountButton {
                text: qsTr("Cancel")
                onClicked: root.close()
                Layout.preferredHeight: root.height * .10
                Layout.preferredWidth: root.width * .47
            }

            AccountButton {
                text: qsTr("Add")
                objectName: "finish"
                Layout.preferredHeight: root.height * .10
                Layout.preferredWidth: root.width * .47

                signal s_clicked()

                onClicked: {
                    entry["type"] = typeComboQuick.currentValue
                    entry["value"] = valueSpin.realValue
                    entry["date"] = dateButton.text
                    entry["category"] = catComboQuick.currentText
                    entry["title"] = title.text

                    s_clicked()
                    root.close()
                }
            }
        }
    }
}

