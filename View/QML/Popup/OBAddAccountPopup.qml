import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../Style" as Style
import "../ComposedComponent" as Composed

// Popup form for creating a new Account: name (Account.number), bank,
// opening date, and interest rate, with a Cancel/Add action pair.
// Presentational only — it doesn't own an AccountListModel or generate an
// id, so the caller builds the actual Account (and decides how to persist
// it) from the accepted() signal, the same way OBAddCategoryPopup and
// OBAddProfilePopup leave ownership to their callers.
Popup {
    id: root

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    padding: Style.OBConstants.leftMargins

    // Shared title column width, same pattern as OBTransactionInfo: the
    // widest of the four field titles, so every DescLine lines up.
    readonly property real titleColumnWidth: Math.max(
        nameLine.titleNaturalWidth, bankLine.titleNaturalWidth,
        openingLine.titleNaturalWidth, interestLine.titleNaturalWidth)

    signal accepted(string name, string bank, date opening, real interest)
    signal rejected()

    onOpened: {
        nameLine.value.text = ""
        bankLine.value.text = ""
        openingButton.selectedDate = new Date(NaN)
        interestLine.value.realValue = 0
    }

    background: Rectangle {
        radius: Style.OBConstants.borderRadius
        border.width: Style.OBConstants.borderWidth
        border.color: Style.OBTheme.palette.outline
        gradient: Style.OBStyle.background
    }

    contentItem: ColumnLayout {
        spacing: Style.OBConstants.verticalSpacing

        Composed.DescLine {
            id: nameLine
            title: qsTr("Name")
            titleWidth: root.titleColumnWidth
            Layout.fillWidth: true
            contentType: Composed.DescLine.ContentType.TextInput
            contentConfig: ({ "placeholderText": qsTr("Name") })
        }

        Composed.DescLine {
            id: bankLine
            title: qsTr("Bank")
            titleWidth: root.titleColumnWidth
            Layout.fillWidth: true
            contentType: Composed.DescLine.ContentType.TextInput
            contentConfig: ({ "placeholderText": qsTr("Bank") })
        }

        Composed.DescLine {
            id: openingLine
            title: qsTr("Opening")
            titleWidth: root.titleColumnWidth
            Layout.fillWidth: true

            Composed.OBDateButton {
                id: openingButton
            }
        }

        Composed.DescLine {
            id: interestLine
            title: qsTr("Interest")
            titleWidth: root.titleColumnWidth
            Layout.fillWidth: true
            contentType: Composed.DescLine.ContentType.SpinBox
            contentConfig: ({ "decimals": 2, "realFrom": 0, "realTo": 100, "realStepSize": 0.25 })
        }

        Composed.OBButtonPair {
            id: actionButtons
            Layout.alignment: Qt.AlignRight
            leftText: qsTr("Cancel")
            rightText: qsTr("Add")
            rightEnabled: nameLine.value.text.length > 0 && openingButton.hasDate
            onLeftClicked: {
                root.rejected()
                root.close()
            }
            onRightClicked: {
                root.accepted(nameLine.value.text, bankLine.value.text, openingButton.selectedDate, interestLine.value.realValue)
                root.close()
            }
        }
    }
}
