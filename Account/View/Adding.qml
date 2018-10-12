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

    onClosed: {
        dateModel.clear()
    }

    onOpened: {
        dateCombo.currentIndex = 0
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
        }


        ComboBox {
            id: dateCombo
            model: dateModel

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
                    }
                }

                highlighted: accountSelect.highlightedIndex === index
            }
        }

        RowLayout {
            Label {
                id: l_new
                text: qsTr("Add new entry")
            }

            TextField {
                id: valueLabel
            }

            DoubleSpinBox {
                id: spinbox
            }

            ComboBox {
                id: type
                objectName: "type"
                model: ["Income", "Outcome"]

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

