import QtQuick
import QtTest

import "../../QML/ComposedComponent" as Composed

TestCase {
    id: testCase
    name: "OBCalendar"
    when: windowShown
    width: 400
    height: 400

    Composed.OBCalendar {
        id: calendar
    }

    function init() {
        while (calendar.viewMode !== Composed.OBCalendar.ViewMode.Days) {
            if (calendar.viewMode === Composed.OBCalendar.ViewMode.Years)
                calendar.selectYear(calendar.displayedYear)
            else
                calendar.selectMonth(calendar.displayedMonth)
        }
        calendar.displayedYear = 2026
        calendar.displayedMonth = 8
        calendar.selectedDates = []
        calendar.dailyTotal = null
        calendar.cumulativeTotal = null
    }

    function test_defaultViewModeIsDays() {
        compare(calendar.viewMode, Composed.OBCalendar.ViewMode.Days)
    }

    function test_dailyTotalAndCumulativeTotalDefaultToNull() {
        compare(calendar.dailyTotal, null)
        compare(calendar.cumulativeTotal, null)
    }

    function test_dailyTotalAndCumulativeTotalAreCallable() {
        calendar.dailyTotal = function (date) { return date.getDate() % 2 === 0 ? 10 : -10 }
        calendar.cumulativeTotal = function (date) { return date.getDate() * 2 }
        compare(calendar.dailyTotal(new Date(2026, 7, 4)), 10)
        compare(calendar.dailyTotal(new Date(2026, 7, 5)), -10)
        compare(calendar.cumulativeTotal(new Date(2026, 7, 4)), 8)
    }

    function test_goToNextMonthRollsOverToNextYear() {
        calendar.displayedYear = 2026
        calendar.displayedMonth = 12
        calendar.goToNextMonth()
        compare(calendar.displayedMonth, 1)
        compare(calendar.displayedYear, 2027)
    }

    function test_goToPreviousMonthRollsOverToPreviousYear() {
        calendar.displayedYear = 2026
        calendar.displayedMonth = 1
        calendar.goToPreviousMonth()
        compare(calendar.displayedMonth, 12)
        compare(calendar.displayedYear, 2025)
    }

    function test_zoomOutOncePicksMonths() {
        calendar.zoomOut()
        compare(calendar.viewMode, Composed.OBCalendar.ViewMode.Months)
    }

    function test_zoomOutTwicePicksYears() {
        calendar.zoomOut()
        calendar.zoomOut()
        compare(calendar.viewMode, Composed.OBCalendar.ViewMode.Years)
    }

    function test_zoomInFromMonthsReturnsToDays() {
        calendar.zoomOut()
        calendar.zoomIn()
        compare(calendar.viewMode, Composed.OBCalendar.ViewMode.Days)
    }

    function test_zoomInFromYearsReturnsToMonths() {
        calendar.zoomOut()
        calendar.zoomOut()
        calendar.zoomIn()
        compare(calendar.viewMode, Composed.OBCalendar.ViewMode.Months)
    }

    function test_zoomInFromDaysIsNoOp() {
        calendar.zoomIn()
        compare(calendar.viewMode, Composed.OBCalendar.ViewMode.Days)
    }

    function test_zoomInDoesNotChangeDisplayedMonthOrYear() {
        calendar.displayedMonth = 3
        calendar.displayedYear = 2030
        calendar.zoomOut()
        calendar.zoomIn()
        compare(calendar.displayedMonth, 3)
        compare(calendar.displayedYear, 2030)
    }

    function test_goToPreviousInMonthsViewChangesYear() {
        calendar.displayedYear = 2026
        calendar.zoomOut()
        calendar.goToPrevious()
        compare(calendar.displayedYear, 2025)
        compare(calendar.viewMode, Composed.OBCalendar.ViewMode.Months)
    }

    function test_goToNextInYearsViewChangesDecade() {
        calendar.displayedYear = 2026
        calendar.zoomOut()
        calendar.zoomOut()
        calendar.goToNext()
        compare(calendar.displayedYear, 2036)
        compare(calendar.viewMode, Composed.OBCalendar.ViewMode.Years)
    }

    function test_selectMonthReturnsToDaysView() {
        calendar.zoomOut()
        calendar.selectMonth(3)
        compare(calendar.displayedMonth, 3)
        compare(calendar.viewMode, Composed.OBCalendar.ViewMode.Days)
    }

    function test_handleMonthTappedSelectsDifferentMonth() {
        calendar.displayedMonth = 8
        calendar.zoomOut()
        calendar.handleMonthTapped(3)
        compare(calendar.displayedMonth, 3)
        compare(calendar.viewMode, Composed.OBCalendar.ViewMode.Days)
    }

    function test_handleMonthTappedOnCurrentMonthZoomsOut() {
        calendar.displayedMonth = 8
        calendar.zoomOut()
        calendar.handleMonthTapped(8)
        compare(calendar.displayedMonth, 8)
        compare(calendar.viewMode, Composed.OBCalendar.ViewMode.Years)
    }

    function test_selectYearReturnsToMonthsView() {
        calendar.zoomOut()
        calendar.zoomOut()
        calendar.selectYear(2030)
        compare(calendar.displayedYear, 2030)
        compare(calendar.viewMode, Composed.OBCalendar.ViewMode.Months)
    }

    function test_decadeStartFloorsToTen() {
        calendar.displayedYear = 2026
        compare(calendar.decadeStart, 2020)
        calendar.displayedYear = 2030
        compare(calendar.decadeStart, 2030)
    }

    function test_selectDaySetsSingleSelection() {
        var day = new Date(2026, 7, 4)
        calendar.selectDay(day)
        compare(calendar.selectedDates.length, 1)
        verify(calendar.isDateSelected(day))
    }

    function test_selectDayReplacesPreviousSingleSelection() {
        calendar.selectDay(new Date(2026, 7, 4))
        calendar.selectDay(new Date(2026, 7, 5))
        compare(calendar.selectedDates.length, 1)
        verify(calendar.isDateSelected(new Date(2026, 7, 5)))
        verify(!calendar.isDateSelected(new Date(2026, 7, 4)))
    }

    function test_selectDayOnSoleSelectedDayDeselectsIt() {
        var day = new Date(2026, 7, 4)
        calendar.selectDay(day)
        calendar.selectDay(day)
        compare(calendar.selectedDates.length, 0)
    }

    function test_shiftClickAddsToSelection() {
        var first = new Date(2026, 7, 4)
        var second = new Date(2026, 7, 10)
        calendar.handleDayTapped(first, false)
        calendar.handleDayTapped(second, true)
        compare(calendar.selectedDates.length, 2)
        verify(calendar.isDateSelected(first))
        verify(calendar.isDateSelected(second))
    }

    function test_shiftClickOnSelectedDayRemovesOnlyThatDay() {
        var first = new Date(2026, 7, 4)
        var second = new Date(2026, 7, 10)
        calendar.handleDayTapped(first, false)
        calendar.handleDayTapped(second, true)
        calendar.handleDayTapped(first, true)
        compare(calendar.selectedDates.length, 1)
        verify(!calendar.isDateSelected(first))
        verify(calendar.isDateSelected(second))
    }

    function test_plainClickAfterShiftSelectionCollapsesToSingle() {
        var first = new Date(2026, 7, 4)
        var second = new Date(2026, 7, 10)
        var third = new Date(2026, 7, 15)
        calendar.handleDayTapped(first, false)
        calendar.handleDayTapped(second, true)
        calendar.handleDayTapped(third, false)
        compare(calendar.selectedDates.length, 1)
        verify(calendar.isDateSelected(third))
    }

    function test_isSameDayIgnoresTime() {
        var a = new Date(2026, 7, 4, 9, 0, 0)
        var b = new Date(2026, 7, 4, 23, 59, 0)
        verify(calendar.isSameDay(a, b))
    }

    function test_isSameDayHandlesNull() {
        compare(calendar.isSameDay(null, new Date()), false)
    }

    function test_headerTextShowsMonthAndYearInDaysView() {
        calendar.displayedYear = 2026
        calendar.displayedMonth = 8
        verify(calendar.headerText.indexOf("2026") !== -1)
    }
}
