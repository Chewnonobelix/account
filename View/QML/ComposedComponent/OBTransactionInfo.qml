import QtQuick

import OpenAccount.Model

import "../Style" as Style
import "../Components" as Comp

// Summary card for a single Transaction: a header with its name and date,
// framed by a golden border, and one DescLine per field relevant to the
// user (amount, movement, payment method, category, description) —
// deliberately leaving out bookkeeping fields (id, accountId, isVisible).
// In readOnly mode every field renders as a plain label; otherwise each
// becomes an editable control that writes straight back to `transaction`.
Item {
    id: root

    property Transaction transaction: null
    property bool readOnly: true
    // Categories are user-defined and shared across transactions, so unlike
    // Movement/Support (fixed enums) this card doesn't own the list itself —
    // the caller supplies the app's CategoryListModel to pick from.
    property CategoryListModel categoryModel: null

    readonly property bool isCredit: root.transaction !== null &&
                                      root.transaction.movement === OpenAccountEnums.Movement.Credit

    // Thicker than the usual control border so the gold ring reads clearly
    // against the inner surface.
    readonly property real borderThickness: Style.OBConstants.borderWidth * 3

    // Shared title column width: the widest of the six field titles (e.g.
    // "Payment method"), so every DescLine lines up and no title is ever
    // clipped — grows automatically if a title changes.
    readonly property real titleColumnWidth: Math.max(
        nameLine.titleNaturalWidth, dateLine.titleNaturalWidth, amountLine.titleNaturalWidth,
        movementLine.titleNaturalWidth, supportLine.titleNaturalWidth, categoryLine.titleNaturalWidth,
        descriptionLine.titleNaturalWidth)

    // Widest row (title column + spacing + value column) drives the card's
    // width; adapts to content instead of a hardcoded guess.
    readonly property real contentWidth: Math.max(
        nameLine.implicitWidth, dateLine.implicitWidth, amountLine.implicitWidth,
        movementLine.implicitWidth, supportLine.implicitWidth, categoryLine.implicitWidth,
        descriptionLine.implicitWidth)

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
            PropertyChanges { target: categoryLoader; sourceComponent: categoryLabel }
            PropertyChanges { target: descriptionLoader; sourceComponent: descriptionLabel }
        },
        State {
            name: "editable"
            PropertyChanges { target: nameLoader; sourceComponent: nameInput }
            PropertyChanges { target: dateLoader; sourceComponent: dateInput }
            PropertyChanges { target: amountLoader; sourceComponent: amountInput }
            PropertyChanges { target: movementLoader; sourceComponent: movementInput }
            PropertyChanges { target: supportLoader; sourceComponent: supportInput }
            PropertyChanges { target: categoryLoader; sourceComponent: categoryInput }
            PropertyChanges { target: descriptionLoader; sourceComponent: descriptionInput }
        }
    ]

    // Shared, stateless combo models for the movement/support fields — one
    // instance each, bound as both the read-only lookup (textAt/indexOfValue)
    // and the editable OBComboBox's model.
    MovementModel {
        id: movementModel
    }
    SupportModel {
        id: supportModel
    }

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
                id: categoryLine
                title: "Category"
                titleWidth: root.titleColumnWidth

                Loader {
                    id: categoryLoader
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
            text: root.transaction ? movementModel.textAt(movementModel.indexOfValue(root.transaction.movement)) : ""
        }
    }
    Component {
        id: movementInput
        Comp.OBComboBox {
            model: movementModel
            currentIndex: root.transaction ? movementModel.indexOfValue(root.transaction.movement) : -1
            onActivated: function (index) { if (root.transaction) root.transaction.movement = movementModel.valueAt(index) }
        }
    }

    Component {
        id: supportLabel
        Comp.OBLabel {
            text: root.transaction ? supportModel.textAt(supportModel.indexOfValue(root.transaction.support)) : ""
        }
    }
    Component {
        id: supportInput
        Comp.OBComboBox {
            model: supportModel
            currentIndex: root.transaction ? supportModel.indexOfValue(root.transaction.support) : -1
            onActivated: function (index) { if (root.transaction) root.transaction.support = supportModel.valueAt(index) }
        }
    }

    Component {
        id: categoryLabel
        Comp.OBLabel {
            text: {
                if (!root.transaction || !root.categoryModel) return ""
                const row = root.categoryModel.indexOf(root.transaction.category)
                const category = row >= 0 ? root.categoryModel.at(row) : null
                return category ? category.name : ""
            }
        }
    }
    Component {
        id: categoryInput
        Comp.OBComboBox {
            model: root.categoryModel
            textRole: "name"
            valueRole: "id"
            currentIndex: root.transaction && root.categoryModel ? root.categoryModel.indexOf(root.transaction.category) : -1
            onActivated: function (index) {
                if (!root.transaction || !root.categoryModel) return
                const category = root.categoryModel.at(index)
                if (category) root.transaction.category = category.id
            }
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
