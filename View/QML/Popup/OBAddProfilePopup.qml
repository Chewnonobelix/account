import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../Style" as Style
import "../Components" as Comp
import "../ComposedComponent" as Composed

// Popup form for creating a new Profile: a first name and last name field
// side by side, with a Cancel/Add action pair. Presentational only — it
// doesn't own a ProfileListModel or generate an id, so the caller builds
// the actual Profile (and decides how to persist it) from the accepted()
// signal, the same way OBAddCategoryPopup leaves Category ownership to
// its caller.
Popup {
    id: root

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    padding: Style.OBConstants.leftMargins

    property alias firstName: firstNameField.text
    property alias lastName: lastNameField.text

    signal accepted(string firstName, string lastName)
    signal rejected()

    onOpened: {
        firstNameField.text = ""
        lastNameField.text = ""
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
                text: qsTr("NAME")
                font.bold: true
            }

            Comp.OBTextInput {
                id: firstNameField
                Layout.fillWidth: true
                Layout.preferredWidth: Style.OBConstants.widthBig
                placeholderText: qsTr("Name")
            }

            Comp.OBLabel {
                text: qsTr("LAST NAME")
                font.bold: true
            }

            Comp.OBTextInput {
                id: lastNameField
                Layout.fillWidth: true
                Layout.preferredWidth: Style.OBConstants.widthBig
                placeholderText: qsTr("Last name")
            }
        }

        Composed.OBButtonPair {
            id: actionButtons
            Layout.alignment: Qt.AlignRight
            leftText: qsTr("Cancel")
            rightText: qsTr("Add")
            rightEnabled: lastNameField.text.length > 0
            onLeftClicked: {
                root.rejected()
                root.close()
            }
            onRightClicked: {
                root.accepted(firstNameField.text, lastNameField.text)
                root.close()
            }
        }
    }
}
