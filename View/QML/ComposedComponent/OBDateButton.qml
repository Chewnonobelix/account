import QtQuick
import QtQuick.Controls

import "../Style" as Style
import "../Components" as Comp

// Button that opens a single-day OBCalendar picker in a popup below it;
// picking a day sets selectedDate and closes the popup. OBCalendar itself
// supports multi-selection (shift-click), but that's not exposed here:
// only a plain click, keeping a single date, is expected.
Item {
    id: root

    property date selectedDate: new Date(NaN)
    property string placeholderText: "Select date"
    property string dateFormat: "yyyy-MM-dd"

    readonly property bool hasDate: !isNaN(root.selectedDate.getTime())

    readonly property alias buttonItem: dateButton
    readonly property alias popup: calendarPopup
    readonly property alias calendar: popupCalendar

    implicitWidth: dateButton.implicitWidth
    implicitHeight: dateButton.implicitHeight

    Comp.OBButton {
        id: dateButton
        anchors.fill: parent
        text: root.hasDate ? Qt.formatDate(root.selectedDate, root.dateFormat) : root.placeholderText
        onClicked: calendarPopup.visible ? calendarPopup.close() : calendarPopup.open()
    }

    // Marks the button as openable; purely decorative, clicks still go to
    // dateButton underneath.
    Comp.OBLabel {
        text: ">"
        x: dateButton.width - width - Style.OBConstants.rightMargins
        y: (dateButton.height - height) / 2
        textState: dateButton.enabled ? Comp.OBLabel.TextState.Neutral : Comp.OBLabel.TextState.Disabled
    }

    Popup {
        id: calendarPopup
        y: dateButton.height
        modal: true
        focus: true
        padding: Style.OBConstants.leftMargins
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        contentItem: OBCalendar {
            id: popupCalendar
            selectedDates: root.hasDate ? [root.selectedDate] : []

            onSelectedDatesChanged: {
                if (popupCalendar.selectedDates.length > 0) {
                    root.selectedDate = popupCalendar.selectedDates[0]
                    calendarPopup.close()
                }
            }
        }

        background: Rectangle {
            radius: Style.OBConstants.borderRadius
            border.width: Style.OBConstants.borderWidth
            border.color: Style.OBTheme.palette.outline
            gradient: Style.OBStyle.background
        }
    }
}
