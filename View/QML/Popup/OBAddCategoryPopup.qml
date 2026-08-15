import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import OpenAccount.Model

import "../Style" as Style
import "../Components" as Comp
import "../ComposedComponent" as Composed

// Popup form for creating a new Category: a name field and a Movement
// picker, with a Cancel/Add action pair. Presentational only — it doesn't
// own a CategoryListModel or generate an id, so the caller builds the
// actual Category (and decides how to persist it) from the accepted()
// signal, the same way OBTransactionInfo leaves category ownership to
// whoever supplies its categoryModel.
Popup {
    id: root

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    padding: Style.OBConstants.leftMargins

    property alias categoryName: nameField.text
    property int direction: OpenAccountEnums.Movement.Both

    signal accepted(string name, int direction)
    signal rejected()

    onOpened: {
        nameField.text = ""
        root.direction = OpenAccountEnums.Movement.Both
    }

    background: Rectangle {
        radius: Style.OBConstants.borderRadius
        border.width: Style.OBConstants.borderWidth
        border.color: Style.OBTheme.palette.outline
        gradient: Style.OBStyle.background
    }

    contentItem: ColumnLayout {
        spacing: Style.OBConstants.verticalSpacing

        RowLayout {
            Layout.fillWidth: true
            spacing: Style.OBConstants.horizontalSpacing

            Comp.OBLabel {
                text: qsTr("CATEGORY")
                font.bold: true
            }

            Comp.OBTextInput {
                id: nameField
                Layout.fillWidth: true
                Layout.preferredWidth: Style.OBConstants.widthBig
                placeholderText: qsTr("Name")
            }

            Comp.OBComboBox {
                id: directionCombo
                model: MovementModel { id: movementModel }
                currentIndex: movementModel.indexOfValue(root.direction)
                onActivated: function (index) { root.direction = movementModel.valueAt(index) }
            }
        }

        Composed.OBButtonPair {
            id: actionButtons
            Layout.alignment: Qt.AlignRight
            leftText: qsTr("Cancel")
            rightText: qsTr("Add")
            rightEnabled: nameField.text.length > 0
            onLeftClicked: {
                root.rejected()
                root.close()
            }
            onRightClicked: {
                root.accepted(nameField.text, root.direction)
                root.close()
            }
        }
    }
}
