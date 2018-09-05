import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.4

Popup {
        id: adding
        closePolicy: Popup.NoAutoClose

        function reset() {
            valueLabel.text = ""
            spinbox.value = 0
        }

        ColumnLayout {
            Label {
                id: labelAdd
                text: qsTr("Adding")
            }


            RowLayout {
                Label {
                    id: l_new
                    text: qsTr("Add new entry")
                }

                TextField {
                    id: valueLabel
                }

                SpinBox {
                    id: spinbox
                    from: 0
                    value: 110
                    to: 100 * 100
                    stepSize: 10
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100

                    validator: DoubleValidator {
                        bottom: Math.min(spinbox.from, spinbox.to)
                        top:  Math.max(spinbox.from, spinbox.to)
                    }

                    textFromValue: function(value, locale) {
                        return Number(value / 100).toLocaleString(locale, 'f', spinbox.decimals)
                    }

                    valueFromText: function(text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }
                }
            }

            RowLayout {
                Button {
                    id: b_save
                    text: qsTr("Save")

                    onClicked: {
                        tableModel.insert(tableModel.count, {"datec": cal.selectedDate.toLocaleDateString(), "value":spinbox.realValue, "labelc": valueLabel.text, "total":0})

                        /*datec: "2018-11-6"
                            value: 25.3
                            labelc: "X text"
                            total: 65.6*/

                       adding.reset()
                        adding.close()


                    }
                }

                Button {
                    id:b_cancel
                    text: qsTr("Cancel")

                    onClicked: {
                        adding.reset()
                        adding.close()
                    }
                }
            }
        }
    }
