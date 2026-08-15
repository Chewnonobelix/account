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
        // Left un-parented, a Popup declared as a plain child of `root`
        // (rather than through a Control's own dedicated `popup:` property,
        // which handles this internally) stays a normal child of `root` for
        // clipping purposes too — so if `root` sits inside a ScrollView/
        // Flickable, the calendar gets cut off at the viewport's edge
        // instead of drawing over it, and any part below that edge is both
        // invisible and unclickable. Parenting to the window's Overlay
        // escapes all ancestor clip: true rectangles.
        parent: Overlay.overlay
        modal: true
        focus: true
        padding: Style.OBConstants.leftMargins
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        // Re-anchored relative to the Overlay instead of `root`, so this
        // recomputes root's on-screen position explicitly. A plain
        // `x/y: root.mapToItem(...)` binding would work initially but not
        // stay correct: mapToItem()'s result doesn't refresh just because
        // an ancestor Flickable scrolled root without root's own x/y/width/
        // height changing, so it's redone on every open instead.
        //
        // Escaping ancestor clipping (above) also means nothing clamps the
        // popup to the window anymore: opened from a button near the
        // bottom/right edge, "just below/left-aligned with the button"
        // could put part or all of the calendar outside the Overlay's own
        // bounds — visually cut off by the window edge and, unlike the
        // clipping case, not scrollable back into view. Flip above the
        // button (or clamp within the Overlay) whenever the natural
        // position wouldn't fit.
        onAboutToShow: {
            const overlay = calendarPopup.parent
            if (!overlay) return

            const buttonTopLeft = root.mapToItem(overlay, 0, 0)
            const belowY = buttonTopLeft.y + root.height
            const fitsBelow = belowY + calendarPopup.height <= overlay.height
            calendarPopup.y = fitsBelow ? belowY : Math.max(0, buttonTopLeft.y - calendarPopup.height)
            calendarPopup.x = Math.min(Math.max(buttonTopLeft.x, 0), Math.max(0, overlay.width - calendarPopup.width))
        }

        contentItem: OBCalendar {
            id: popupCalendar
            selectedDates: root.hasDate ? [root.selectedDate] : []

            onSelectedDatesChanged: {
                if (popupCalendar.selectedDates.length === 0) return

                const picked = popupCalendar.selectedDates[0]
                // selectedDates above is itself driven by root.selectedDate,
                // so this handler also fires when selectedDate changes from
                // the *outside* (e.g. a caller's `selectedDate:
                // someModel.startDate` binding re-evaluating). Writing back
                // unconditionally there — even the exact same date — would
                // still clear that binding: any imperative assignment does,
                // regardless of whether the value actually changed. Skipping
                // the no-op case keeps the binding alive until the user
                // genuinely picks a different day.
                if (!root.hasDate || picked.getTime() !== root.selectedDate.getTime())
                    root.selectedDate = picked

                calendarPopup.close()
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
