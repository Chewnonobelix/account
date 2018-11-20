import QtQuick 2.11
import QtQml 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4

Popup {
//    closePolicy: Popup.NoAutoClose

    function reset() {
        valueLabel.text = ""
        spinbox.value = 0
        dateModel.clear()
        dateCombo.currentIndex = 0
    }

    AccountStyle {
        id: pageStyle
    }


    signal accept()

    property real v_val: spinbox.realValue
    property string v_title: valueLabel.text
    property string v_type: type.currentText
    property string v_date: dateCombo.currentText
    property bool newAccount: false

    onClosed: {
        dateModel.clear()
    }

    onOpened: {
        dateCombo.currentIndex = 0
        l_new.text = newAccount ? qsTr("Add new account"): qsTr("Add new entry")
    }

    background: Rectangle {
        gradient: pageStyle.backgroundGradient
        border.color: "darkgoldenrod"
    }



//    property var dateModel: []

    ListModel {
        id: dateModel
    }

    function addDate(d) {
        dateModel.append({"text:": d})
    }

    ColumnLayout {
        Label {
            id: labelAdd
            text: qsTr("Adding " + v_date)
            font.family: pageStyle.title.name
            font.pixelSize: pageStyle.title.size
        }


        ComboBox {
            id: dateCombo
            model: dateModel
            font.family: pageStyle.core.name
            font.pixelSize: pageStyle.core.size
            Rectangle {
                gradient: pageStyle.goldButton
                anchors.fill: parent
            }

            delegate: ItemDelegate {
                width: accountSelect.width

                contentItem: Rectangle {
                    gradient: pageStyle.goldButton
                    anchors.fill: parent
                    Label {
                        color: "black"
                        text: modelData
                        anchors.centerIn: parent
                        font.family: pageStyle.core.name
                        font.pixelSize: pageStyle.core.size
                    }
                }

                highlighted: accountSelect.highlightedIndex === index
            }
        }

        RowLayout {
            Label {
                id: l_new
                text: qsTr("Add new entry")
                font.family: pageStyle.title.name
                font.pixelSize: pageStyle.title.size
            }

            TextField {
                id: valueLabel
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
            }

            DoubleSpinBox {
                id: spinbox
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
            }

            ComboBox {
                id: type
                objectName: "type"
                model: ["Income", "Outcome"]
                enabled: !addingid.newAccount

                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size

                Rectangle {
                    gradient: pageStyle.goldButton
                    anchors.fill: parent
                }

                delegate: ItemDelegate {
                    width: accountSelect.width

                    contentItem: Rectangle {
                        gradient: pageStyle.goldButton
                        anchors.fill: parent
                        Label {
                            color: "black"
                            text: modelData
                            anchors.centerIn: parent
                            font.family: pageStyle.core.name
                            font.pixelSize: pageStyle.core.size
                        }
                    }

                    highlighted: accountSelect.highlightedIndex === index
                }
            }
        }

        RowLayout {
            Button {
                id: b_save
                text: qsTr("Save")
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size

                background: Rectangle {
                    gradient: pageStyle.goldButton
                    id: saveRect
                }


                onPressed: {
                    saveRect.gradient = pageStyle.darkGoldButton
                }

                onReleased: {
                    saveRect.gradient = pageStyle.goldButton
                    addingid.accept()
                    reset()
                    close()
                }
            }

            Button {
                id:b_cancel
                text: qsTr("Cancel")
                font.family: pageStyle.core.name
                font.pixelSize: pageStyle.core.size
                background: Rectangle {
                    gradient: pageStyle.goldButton
                    id: cancelRect
                }

                onPressed: {
                    cancelRect.gradient = pageStyle.darkGoldButton
                }

                onReleased: {
                    cancelRect.gradient = pageStyle.goldButton
                    reset()
                    close()
                }
            }
        }
    }
}

