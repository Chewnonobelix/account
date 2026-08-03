import QtQuick
import QtQuick.Controls

import OpenAccount.Model

import "../../View/QML/Style" as Style
import "../../View/QML/Components" as Comp
import "../../View/QML/ComposedComponent" as Composed

ApplicationWindow {
    id: window

    component OBTabButton: TabButton {
        id: tabButton

        implicitWidth: Style.OBConstants.widthMedium
        implicitHeight: Style.OBConstants.heightMedium

        background: Rectangle {
            radius: Style.OBConstants.borderRadius
            border.width: Style.OBConstants.borderWidth
            border.color: !tabButton.enabled ? Style.OBTheme.palette.disabled :
                          (tabButton.checked || tabButton.down) ? Style.OBTheme.palette.outlineStrong : Style.OBTheme.palette.outline
            gradient: !tabButton.enabled ? Style.OBStyle.silver :
                      (tabButton.checked || tabButton.down) ? Style.OBStyle.goldIn : Style.OBStyle.goldOut
        }

        contentItem: Comp.OBLabel {
            text: tabButton.text
            horizontalAlignment: Text.AlignHCenter
            textState: tabButton.enabled ? Comp.OBLabel.TextState.Neutral : Comp.OBLabel.TextState.Disabled
        }
    }

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

    TabBar {
        id: tabBar
        anchors.top: headerRow.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Style.OBConstants.leftMargins

        OBTabButton {
            text: "Components"
        }
        OBTabButton {
            text: "Complex components"
        }
    }

    ScrollView {
        id: componentsTab
        visible: tabBar.currentIndex === 0
        anchors.top: tabBar.bottom
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
                    orientation: headerOrientationBox.currentIndex === 0 ? Gradient.Vertical : Gradient.Horizontal
                    reversed: headerReversedToggle.checked
                }
                Comp.OBTextInput {
                    id: headerTextInput
                    text: "Section title"
                }
                Comp.OBComboBox {
                    id: headerOrientationBox
                    model: ["Vertical", "Horizontal"]
                }
                Comp.OBCheckBox {
                    id: headerReversedToggle
                    text: "reversed"
                    checked: false
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            UiKitSection {
                title: "OBTitle"
                target: demoTitle
                width: parent.width

                Comp.OBTitle {
                    id: demoTitle
                    text: titleTextInput.text
                    anchors.verticalCenter: parent.verticalCenter
                }
                Comp.OBTextInput {
                    id: titleTextInput
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

    ScrollView {
        id: complexComponentsTab
        visible: tabBar.currentIndex === 1
        anchors.top: tabBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: Style.OBConstants.leftMargins
        clip: true

        Column {
            width: window.width - 2 * Style.OBConstants.leftMargins
            spacing: Style.OBConstants.verticalSpacing * 2

            UiKitSection {
                title: "DescLine"
                width: parent.width

                Column {
                    spacing: Style.OBConstants.verticalSpacing / 2

                    Composed.DescLine {
                        title: "Account name"
                        contentType: Composed.DescLine.ContentType.TextInput
                        contentConfig: ({ "text": "Checking" })
                    }

                    Composed.DescLine {
                        title: "Active"
                        contentType: Composed.DescLine.ContentType.CheckBox
                        contentConfig: ({ "checked": true })
                    }

                    Composed.DescLine {
                        title: "Balance"
                        contentType: Composed.DescLine.ContentType.SpinBox
                        contentConfig: ({ "realTo": 1000, "realValue": 250 })
                    }

                    Composed.DescLine {
                        title: "Status"
                        contentType: Composed.DescLine.ContentType.Label
                        contentConfig: ({ "text": "Ok", "textState": Comp.OBLabel.TextState.Positive })
                    }

                    Composed.DescLine {
                        title: "Type"
                        contentType: Composed.DescLine.ContentType.ComboBox
                        contentConfig: ({ "model": ["Checking", "Savings", "Credit card", "Cash"] })
                    }

                    // Explicit-content mode: any custom Item works as the value slot too.
                    Composed.DescLine {
                        title: "Custom content"

                        Row {
                            spacing: Style.OBConstants.horizontalSpacing / 2

                            Comp.OBButton {
                                text: "Edit"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                }
            }

            UiKitSection {
                title: "TableRow"
                width: parent.width

                Column {
                    spacing: Style.OBConstants.verticalSpacing / 2

                    Composed.TableRow {
                        width: Style.OBConstants.widthBig * 1.5
                        text: "Groceries — Neutral"
                        selected: true
                        rowState: Composed.TableRow.RowState.Neutral
                    }

                    Composed.TableRow {
                        width: Style.OBConstants.widthBig * 1.5
                        text: "Salary — Positive"
                        selected: true
                        rowState: Composed.TableRow.RowState.Positive
                    }

                    Composed.TableRow {
                        width: Style.OBConstants.widthBig * 1.5
                        text: "Overdraft fee — Negative"
                        selected: true
                        rowState: Composed.TableRow.RowState.Negative
                    }

                    Composed.TableRow {
                        width: Style.OBConstants.widthBig * 1.5
                        text: "Tap me to select"
                    }
                }
            }

            UiKitSection {
                title: "TransactionRow"
                width: parent.width

                Column {
                    spacing: Style.OBConstants.verticalSpacing / 2

                    Composed.TransactionRow {
                        transaction: Transaction {
                            name: "Salary"
                            description: "Monthly salary"
                            date: "2026-01-05"
                            value: 2500
                            movement: OpenAccountEnums.Movement.Credit
                        }
                    }

                    Composed.TransactionRow {
                        transaction: Transaction {
                            name: "Rent"
                            description: "Monthly rent"
                            date: "2026-01-06"
                            value: 950
                            movement: OpenAccountEnums.Movement.Debit
                        }
                    }
                }
            }

            Item { width: 1; height: Style.OBConstants.bottomMargins }
        }
    }
}
