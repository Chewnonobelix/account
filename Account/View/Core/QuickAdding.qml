import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "../Style"

Window {
    height: 500
    width: height * 9 / 16
    AccountStyle {
        id: pageStyle
    }

    CoreModel {
        id: cModels
    }

    flags: Qt.Popup

    onActiveChanged: if(!active) close()
    id: root
    property var entry: Object()

    property var incomeCats: []
    property var outcomeCats: []

    Rectangle {
        anchors.fill: parent
        gradient: pageStyle.backgroundGradient
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

            Label {
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .96
                Layout.columnSpan: 2
                text: qsTr("Quick adding")
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                fontSizeMode: Text.Fit
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                background: Rectangle {
                    gradient: pageStyle.goldHeader
                }

            }

            ComboBox {
                model: cModels.typeModel
                id: typeComboQuick
                objectName: "type"
                valueRole: "type"
                textRole: "name"

                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47

                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size

                ToolTip.text: qsTr("Specify income or outcome")
                ToolTip.visible: hovered
                ToolTip.delay: 500

                background:  Rectangle {
                    gradient: pageStyle.goldButton
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
                        gradient: pageStyle.goldButton
                        anchors.fill: parent
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            acceptedButtons: Qt.NoButton
                        }
                        Label {
                            color: "black"
                            text: name
                            anchors.centerIn: parent
                            font.family: pageStyle.core.name
                            font.pixelSize: pageStyle.core.size
                        }
                    }

                    highlighted: typeComboQuick.highlightedIndex === index
                }
            }

            Label {
                text: qsTr("Type")
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47

                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                fontSizeMode: Text.Fit
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter

                background: Rectangle {
                    gradient: pageStyle.goldHeader
                }
            }

            CalendarButton {
                id: dateButton
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47
            }

            Label {
                text: qsTr("Date")
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                fontSizeMode: Text.Fit
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                background: Rectangle {
                    gradient: pageStyle.goldHeader
                }
            }

            TextField {
                id: title
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47
            }

            Label {
                text: qsTr("Title")
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                fontSizeMode: Text.Fit
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                background: Rectangle {
                    gradient: pageStyle.goldHeader
                }
            }

            DoubleSpinBox {
                id: valueSpin
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47
            }

            Label {
                text: qsTr("Value")
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                fontSizeMode: Text.Fit
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                background: Rectangle {
                    gradient: pageStyle.goldHeader
                }
            }

            CategoryItem{
                id: catComboQuick
                objectName: "cat"
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47
                Component.onCompleted: model = Qt.binding(function() {return typeComboQuick.currentValue === "income" ? root.incomeCats : root.outcomeCats})
            }

            Label {
                text: qsTr("Category")
                Layout.preferredHeight: root.height * .15
                Layout.preferredWidth: root.width * .47
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
                fontSizeMode: Text.Fit
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                background: Rectangle {
                    gradient: pageStyle.goldHeader
                }
            }

            Button {
                text: qsTr("Cancel")
                onClicked: root.close()
                Layout.preferredHeight: root.height * .10
                Layout.preferredWidth: root.width * .47

                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size

                background: Rectangle {
                    anchors.fill: parent
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.NoButton
                        cursorShape: Qt.PointingHandCursor
                    }
                }

            }

            Button {
                text: qsTr("Add")
                objectName: "finish"
                Layout.preferredHeight: root.height * .10
                Layout.preferredWidth: root.width * .47

                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size

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
                background: Rectangle {
                    anchors.fill: parent
                    gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.NoButton
                        cursorShape: Qt.PointingHandCursor
                    }
                }
            }
        }
    }
}
