import QtQuick
import QtQuick.Controls

import "../../View/QML/Style" as Style
import "../../View/QML/Components" as Comp

ApplicationWindow {
    id: window

    width: 560
    height: 820
    visible: true
    title: "OpenAccount — UiKit"
    color: Style.OBTheme.palette.backgroundStart

    Row {
        id: headerRow
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Style.OBConstants.leftMargins
        height: Style.OBConstants.heightBig
        spacing: Style.OBConstants.horizontalSpacing

        Comp.OBLabel {
            text: "OpenAccount UiKit"
            font.bold: true
            font.pixelSize: 20
            anchors.verticalCenter: parent.verticalCenter
            width: headerRow.width - themeRow.width - headerRow.spacing
        }

        Row {
            id: themeRow
            anchors.verticalCenter: parent.verticalCenter
            spacing: Style.OBConstants.horizontalSpacing / 2

            Comp.OBLabel {
                text: "Night mode"
                anchors.verticalCenter: parent.verticalCenter
            }

            Comp.OBSwitch {
                id: themeSwitch
                anchors.verticalCenter: parent.verticalCenter
                checked: Style.OBTheme.isNight
                onCheckedChanged: Style.OBTheme.mode = checked ? Style.OBTheme.Mode.Night : Style.OBTheme.Mode.Day
            }
        }
    }

    ScrollView {
        anchors.top: headerRow.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: Style.OBConstants.leftMargins
        clip: true

        Column {
            width: window.width - 2 * Style.OBConstants.leftMargins
            spacing: Style.OBConstants.verticalSpacing * 2

            UiKitSection {
                title: "OBButton"
                target: demoButton
                width: parent.width

                Comp.OBButton {
                    id: demoButton
                    text: buttonTextInput.text
                }
                Comp.OBTextInput {
                    id: buttonTextInput
                    text: "Click me"
                }
            }

            UiKitSection {
                title: "OBLabel"
                target: demoLabel
                width: parent.width

                Comp.OBLabel {
                    id: demoLabel
                    text: labelTextInput.text
                    textState: labelStateBox.currentIndex
                    anchors.verticalCenter: parent.verticalCenter
                }
                Comp.OBTextInput {
                    id: labelTextInput
                    text: "Sample label"
                }
                Comp.OBComboBox {
                    id: labelStateBox
                    model: ["Neutral", "Positive", "Negative", "Disabled"]
                }
            }

            UiKitSection {
                title: "OBHeader"
                target: demoHeader
                width: parent.width

                Comp.OBHeader {
                    id: demoHeader
                    text: headerTextInput.text
                    width: Style.OBConstants.widthBig
                }
                Comp.OBTextInput {
                    id: headerTextInput
                    text: "Section title"
                }
            }

            UiKitSection {
                title: "OBTextInput"
                target: demoInput
                width: parent.width

                Comp.OBTextInput {
                    id: demoInput
                    placeholderText: "Type here"
                }
                Comp.OBLabel {
                    text: "-> " + demoInput.text
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            UiKitSection {
                title: "OBSpinBox (real numbers)"
                target: demoSpin
                width: parent.width

                Comp.OBSpinBox {
                    id: demoSpin
                    decimals: 2
                    realFrom: 0
                    realTo: 100
                    realStepSize: 0.5
                    realValue: 10
                }
                Comp.OBLabel {
                    text: "realValue = " + demoSpin.realValue.toFixed(2)
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            UiKitSection {
                title: "OBSwitch (tristate)"
                target: demoSwitch
                width: parent.width

                Comp.OBSwitch {
                    id: demoSwitch
                    tristate: switchTristateToggle.checked
                    anchors.verticalCenter: parent.verticalCenter
                }
                Comp.OBCheckBox {
                    id: switchTristateToggle
                    text: "tristate"
                    checked: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                Comp.OBLabel {
                    text: demoSwitch.checkState === Qt.Unchecked ? "Unchecked" :
                          demoSwitch.checkState === Qt.PartiallyChecked ? "Partial" : "Checked"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            UiKitSection {
                title: "OBCheckBox"
                target: demoCheck
                width: parent.width

                Comp.OBCheckBox {
                    id: demoCheck
                    text: "Accept terms"
                    tristate: checkTristateToggle.checked
                    anchors.verticalCenter: parent.verticalCenter
                }
                Comp.OBCheckBox {
                    id: checkTristateToggle
                    text: "tristate"
                    checked: false
                    anchors.verticalCenter: parent.verticalCenter
                }
                Comp.OBLabel {
                    text: demoCheck.checkState === Qt.Unchecked ? "Unchecked" :
                          demoCheck.checkState === Qt.PartiallyChecked ? "Partial" : "Checked"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            UiKitSection {
                title: "OBComboBox"
                target: demoCombo
                width: parent.width

                Comp.OBComboBox {
                    id: demoCombo
                    model: ["Checking", "Savings", "Credit card", "Cash"]
                }
                Comp.OBLabel {
                    text: "-> " + demoCombo.displayText
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Item { width: 1; height: Style.OBConstants.bottomMargins }
        }
    }
}
