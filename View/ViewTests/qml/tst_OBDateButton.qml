import QtQuick
import QtTest

import "../../QML/ComposedComponent" as Composed

TestCase {
    id: testCase
    name: "OBDateButton"
    when: windowShown
    width: 400
    height: 400

    Composed.OBDateButton {
        id: dateButton
    }

    function init() {
        dateButton.selectedDate = new Date(NaN)
        dateButton.popup.close()
    }

    function test_noDateShowsPlaceholder() {
        compare(dateButton.hasDate, false)
        compare(dateButton.buttonItem.text, dateButton.placeholderText)
    }

    function test_selectedDateShowsFormattedInButtonText() {
        dateButton.selectedDate = new Date(2026, 7, 4)
        compare(dateButton.hasDate, true)
        compare(dateButton.buttonItem.text, Qt.formatDate(dateButton.selectedDate, dateButton.dateFormat))
    }

    function test_clickingButtonOpensPopup() {
        dateButton.buttonItem.clicked()
        compare(dateButton.popup.opened, true)
    }

    function test_pickingDayInCalendarSetsSelectedDateAndClosesPopup() {
        dateButton.popup.open()
        var picked = new Date(2026, 7, 15)
        dateButton.calendar.selectDay(picked)
        compare(dateButton.calendar.isSameDay(dateButton.selectedDate, picked), true)
        compare(dateButton.popup.opened, false)
    }
}
