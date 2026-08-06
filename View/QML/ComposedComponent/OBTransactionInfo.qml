import QtQuick

import OpenAccount.Model

import "../Style" as Style
import "../Components" as Comp

// Summary card for a single Transaction: a header with its name and date,
// framed by a golden border, and one DescLine per field relevant to the
// user (amount, movement, payment method, description) — deliberately
// leaving out bookkeeping fields (id, accountId, category, isVisible).
// In readOnly mode every field renders as a plain label; otherwise each
// becomes an editable control that writes straight back to `transaction`.
Item {
    id: root

    property Transaction transaction: null
    property bool readOnly: true

    readonly property bool isCredit: root.transaction !== null &&
                                      root.transaction.movement === OpenAccountEnums.Movement.Credit

    readonly property var movementNames: ["Credit", "Debit"]
    readonly property var movementValues: [OpenAccountEnums.Movement.Credit, OpenAccountEnums.Movement.Debit]
    readonly property var supportNames: ["Cash", "Cheque", "Card", "Transfer", "Other"]
    readonly property var supportValues: [OpenAccountEnums.Support.Cash, OpenAccountEnums.Support.Cheque,
                                           OpenAccountEnums.Support.Card, OpenAccountEnums.Support.Transfer,
                                           OpenAccountEnums.Support.Other]

    // Thicker than the usual control border so the gold ring reads clearly
    // against the inner surface.
    readonly property real borderThickness: Style.OBConstants.borderWidth * 3

    // Shared title column width: the widest of the six field titles (e.g.
    // "Payment method"), so every DescLine lines up and no title is ever
    // clipped — grows automatically if a title changes.
    readonly property real titleColumnWidth: Math.max(
        nameLine.titleNaturalWidth, dateLine.titleNaturalWidth, amountLine.titleNaturalWidth,
        movementLine.titleNaturalWidth, supportLine.titleNaturalWidth, descriptionLine.titleNaturalWidth)

    // Widest row (title column + spacing + value column) drives the card's
    // width; adapts to content instead of a hardcoded guess.
    readonly property real contentWidth: Math.max(
        nameLine.implicitWidth, dateLine.implicitWidth, amountLine.implicitWidth,
        movementLine.implicitWidth, supportLine.implicitWidth, descriptionLine.implicitWidth)

    implicitWidth: root.contentWidth + 2 * Style.OBConstants.leftMargins + 2 * root.borderThickness
    implicitHeight: contentColumn.implicitHeight + 2 * Style.OBConstants.topMargins + 2 * root.borderThickness

    state: root.readOnly ? "readOnly" : "editable"

    states: [
        State {
            name: "readOnly"
            PropertyChanges { target: nameLoader; sourceComponent: nameLabel }
            PropertyChanges { target: dateLoader; sourceComponent: dateLabel }
            PropertyChanges { target: amountLoader; sourceComponent: amountLabel }
            PropertyChanges { target: movementLoader; sourceComponent: movementLabel }
            PropertyChanges { target: supportLoader; sourceComponent: supportLabel }
            PropertyChanges { target: descriptionLoader; sourceComponent: descriptionLabel }
        },
        State {
            name: "editable"
            PropertyChanges { target: nameLoader; sourceComponent: nameInput }
            PropertyChanges { target: dateLoader; sourceComponent: dateInput }
            PropertyChanges { target: amountLoader; sourceComponent: amountInput }
            PropertyChanges { target: movementLoader; sourceComponent: movementInput }
            PropertyChanges { target: supportLoader; sourceComponent: supportInput }
            PropertyChanges { target: descriptionLoader; sourceComponent: descriptionInput }
        }
    ]

    // Golden frame: an outer rectangle painted with the gold gradient, left
    // showing only as a ring since the inner rectangle covers everything
    // but that margin.
    Rectangle {
        anchors.fill: parent
        radius: Style.OBConstants.borderRadius
        gradient: Style.OBStyle.goldOut
    }

    Rectangle {
        anchors.fill: parent
        anchors.margins: root.borderThickness
        radius: Style.OBConstants.borderRadius
        gradient: Style.OBStyle.background

        Column {
            id: contentColumn
            anchors.fill: parent
            anchors.margins: Style.OBConstants.leftMargins
            spacing: Style.OBConstants.verticalSpacing

            Comp.OBHeader {
                width: root.contentWidth
                text: root.transaction ? root.transaction.name + " - " + Qt.formatDate(root.transaction.date, "yyyy-MM-dd") : ""
            }

            Rectangle {
                width: root.contentWidth
                height: 1
                color: Style.OBTheme.palette.outline
                opacity: 0.3
            }

            DescLine {
                id: nameLine
                title: "Name"
                titleWidth: root.titleColumnWidth

                Loader {
                    id: nameLoader
                }
            }

            DescLine {
                id: dateLine
                title: "Date"
                titleWidth: root.titleColumnWidth

                Loader {
                    id: dateLoader
                }
            }

            DescLine {
                id: amountLine
                title: "Amount"
                titleWidth: root.titleColumnWidth

                Loader {
                    id: amountLoader
                }
            }

            DescLine {
                id: movementLine
                title: "Movement"
                titleWidth: root.titleColumnWidth

                Loader {
                    id: movementLoader
                }
            }

            DescLine {
                id: supportLine
                title: "Payment method"
                titleWidth: root.titleColumnWidth

                Loader {
                    id: supportLoader
                }
            }

            DescLine {
                id: descriptionLine
                title: "Description"
                titleWidth: root.titleColumnWidth

                Loader {
                    id: descriptionLoader
                }
            }
        }
    }

    Component {
        id: nameLabel
        Comp.OBLabel {
            text: root.transaction ? root.transaction.name : ""
        }
    }
    Component {
        id: nameInput
        Comp.OBTextInput {
            text: root.transaction ? root.transaction.name : ""
            onEditingFinished: if (root.transaction) root.transaction.name = text
        }
    }

    Component {
        id: dateLabel
        Comp.OBLabel {
            text: root.transaction ? Qt.formatDate(root.transaction.date, "yyyy-MM-dd") : ""
        }
    }
    Component {
        id: dateInput
        OBDateButton {
            selectedDate: root.transaction ? root.transaction.date : new Date(NaN)
            onSelectedDateChanged: if (root.transaction) root.transaction.date = selectedDate
        }
    }

    Component {
        id: amountLabel
        Comp.OBLabel {
            text: (root.isCredit ? "+" : "-") + (root.transaction ? Math.abs(root.transaction.value).toFixed(2) : "0.00")
            textState: root.isCredit ? Comp.OBLabel.TextState.Positive : Comp.OBLabel.TextState.Negative
        }
    }
    Component {
        id: amountInput
        Comp.OBSpinBox {
            decimals: 2
            realFrom: 0
            realTo: 1000000
            realValue: root.transaction ? Math.abs(root.transaction.value) : 0
            onRealValueChanged: if (root.transaction) root.transaction.value = realValue
        }
    }

    Component {
        id: movementLabel
        Comp.OBLabel {
            text: root.transaction ? root.movementNames[root.movementValues.indexOf(root.transaction.movement)] : ""
        }
    }
    Component {
        id: movementInput
        Comp.OBComboBox {
            model: root.movementNames
            currentIndex: root.transaction ? root.movementValues.indexOf(root.transaction.movement) : -1
            onActivated: function (index) { if (root.transaction) root.transaction.movement = root.movementValues[index] }
        }
    }

    Component {
        id: supportLabel
        Comp.OBLabel {
            text: root.transaction ? root.supportNames[root.supportValues.indexOf(root.transaction.support)] : ""
        }
    }
    Component {
        id: supportInput
        Comp.OBComboBox {
            model: root.supportNames
            currentIndex: root.transaction ? root.supportValues.indexOf(root.transaction.support) : -1
            onActivated: function (index) { if (root.transaction) root.transaction.support = root.supportValues[index] }
        }
    }

    Component {
        id: descriptionLabel
        Comp.OBLabel {
            text: root.transaction ? root.transaction.description : ""
        }
    }
    Component {
        id: descriptionInput
        Comp.OBTextInput {
            text: root.transaction ? root.transaction.description : ""
            onEditingFinished: if (root.transaction) root.transaction.description = text
        }
    }
}
