import QtQuick
import QtQuick.Controls

import "../Style" as Style
import "../Components" as Comp

// Month calendar with drill-down navigation: day grid -> month grid ->
// decade (year) grid, managed by a StackView (zooming out pushes a level,
// picking a month/year pops back). Clicking the header label zooms out one
// level; previous/next page through whatever unit the current level shows
// (month in the Days view, year in the Months view, decade in the Years
// view). The day grid, weekday header and week-number column are the
// standard Qt Quick Controls MonthGrid / DayOfWeekRow / WeekNumberColumn,
// themed with custom delegates; the month and decade pickers are
// hand-rolled since Qt Quick Controls has no built-in equivalent for them.
Item {
    id: root

    enum ViewMode {
        Days,
        Months,
        Years
    }

    readonly property int cellSize: Style.OBConstants.widthLittle / 2

    // Week-number column + its spacing to the month grid + 7 day columns.
    // The header row is pinned to this so it never drifts from the grid's
    // actual rendered width (Qt's WeekNumberColumn/DayOfWeekRow controls
    // don't reliably expose an implicit width to size against).
    readonly property int weekColumnWidth: root.cellSize
    readonly property int daysGridWidth: root.weekColumnWidth + 2 + root.cellSize * 7
    // Weekday header row + spacing + the 6 week rows.
    readonly property int daysGridHeight: root.cellSize + 2 + root.cellSize * 6

    // The month/decade pickers are 4x3 grids of cells; size them so their
    // overall footprint exactly matches the day grid's, regardless of view.
    readonly property int pickerColumns: 4
    readonly property int pickerRows: 3
    readonly property real pickerCellWidth: (root.daysGridWidth - (root.pickerColumns - 1) * 2) / root.pickerColumns
    readonly property real pickerCellHeight: (root.daysGridHeight - (root.pickerRows - 1) * 2) / root.pickerRows

    property var selectedDates: []
    property int displayedYear: (new Date()).getFullYear()
    property int displayedMonth: (new Date()).getMonth() + 1

    // Optional per-day total indicators, shown as thin bars on the
    // current-month day cells. OBCalendar has no notion of transactions or
    // accounts itself, so the caller supplies these as plain functions
    // (date -> signed number); either can be left null to omit that bar.
    // dailyTotal: that single day's total. cumulativeTotal: the running
    // total from the period start through that day.
    property var dailyTotal: null
    property var cumulativeTotal: null

    readonly property int viewMode: {
        if (calendarStack.depth >= 3)
            return OBCalendar.ViewMode.Years
        if (calendarStack.depth === 2)
            return OBCalendar.ViewMode.Months
        return OBCalendar.ViewMode.Days
    }

    // Decade shown by the Years view: displayedYear=2026 -> 2020, grid then
    // spans 2019 (context) .. 2030 (context) with 2020-2029 as the decade.
    readonly property int decadeStart: Math.floor(root.displayedYear / 10) * 10

    readonly property string headerText: {
        if (root.viewMode === OBCalendar.ViewMode.Days)
            return Qt.locale().monthName(root.displayedMonth - 1, Locale.LongFormat) + " " + root.displayedYear
        if (root.viewMode === OBCalendar.ViewMode.Months)
            return String(root.displayedYear)
        return root.decadeStart + " - " + (root.decadeStart + 9)
    }

    implicitWidth: bodyColumn.implicitWidth
    implicitHeight: bodyColumn.implicitHeight

    function isSameDay(a, b) {
        if (!a || !b)
            return false
        return a.getFullYear() === b.getFullYear() && a.getMonth() === b.getMonth() && a.getDate() === b.getDate()
    }

    function goToPreviousMonth() {
        if (root.displayedMonth === 1) {
            root.displayedMonth = 12
            root.displayedYear -= 1
        } else {
            root.displayedMonth -= 1
        }
    }

    function goToNextMonth() {
        if (root.displayedMonth === 12) {
            root.displayedMonth = 1
            root.displayedYear += 1
        } else {
            root.displayedMonth += 1
        }
    }

    function goToPrevious() {
        if (root.viewMode === OBCalendar.ViewMode.Days)
            root.goToPreviousMonth()
        else if (root.viewMode === OBCalendar.ViewMode.Months)
            root.displayedYear -= 1
        else
            root.displayedYear -= 10
    }

    function goToNext() {
        if (root.viewMode === OBCalendar.ViewMode.Days)
            root.goToNextMonth()
        else if (root.viewMode === OBCalendar.ViewMode.Months)
            root.displayedYear += 1
        else
            root.displayedYear += 10
    }

    function zoomOut() {
        if (root.viewMode === OBCalendar.ViewMode.Days)
            calendarStack.push(monthsViewComponent)
        else if (root.viewMode === OBCalendar.ViewMode.Months)
            calendarStack.push(yearsViewComponent)
    }

    // Reverse of zoomOut: just narrows the view back down without picking a
    // specific month/year (unlike selectMonth/selectYear).
    function zoomIn() {
        if (root.viewMode !== OBCalendar.ViewMode.Days)
            calendarStack.pop()
    }

    MouseArea {
        // MouseArea rather than WheelHandler: the newer Pointer Handler API
        // (TapHandler/WheelHandler) turned out unreliable for this
        // component in practice, so the whole calendar uses the older,
        // more battle-tested MouseArea/onWheel mechanism instead.
        anchors.fill: parent
        acceptedButtons: Qt.NoButton
        onWheel: (wheel) => {
            if (wheel.angleDelta.y < 0)
                root.zoomOut()
            else if (wheel.angleDelta.y > 0)
                root.zoomIn()
            wheel.accepted = true
        }
    }

    function isDateSelected(date) {
        for (var i = 0; i < root.selectedDates.length; i++) {
            if (root.isSameDay(root.selectedDates[i], date))
                return true
        }
        return false
    }

    // Plain click: replace the whole selection with just this date, or
    // clear it if this date was already the sole selection (deselect).
    function selectDay(date) {
        if (root.isDateSelected(date) && root.selectedDates.length === 1)
            root.selectedDates = []
        else
            root.selectedDates = [date]
    }

    // Shift+click: add this date to the selection, or remove it if it was
    // already part of a multi-selection.
    function toggleDayInSelection(date) {
        if (root.isDateSelected(date))
            root.selectedDates = root.selectedDates.filter(function (d) { return !root.isSameDay(d, date) })
        else
            root.selectedDates = root.selectedDates.concat([date])
    }

    function handleDayTapped(date, shiftHeld) {
        if (shiftHeld)
            root.toggleDayInSelection(date)
        else
            root.selectDay(date)
    }

    function selectMonth(month) {
        root.displayedMonth = month
        calendarStack.pop()
    }

    // Tapping the currently displayed month again is a no-op as a
    // selection, so it zooms out to the Years view instead.
    function handleMonthTapped(month) {
        if (month === root.displayedMonth)
            root.zoomOut()
        else
            root.selectMonth(month)
    }

    function selectYear(year) {
        root.displayedYear = year
        calendarStack.pop()
    }

    Column {
        id: bodyColumn
        spacing: Style.OBConstants.verticalSpacing / 2

        Row {
            id: headerRow
            width: root.daysGridWidth
            spacing: Style.OBConstants.horizontalSpacing / 2

            Comp.OBButton {
                id: previousButton
                text: "‹"
                implicitWidth: root.cellSize
                anchors.verticalCenter: parent.verticalCenter
                onClicked: root.goToPrevious()
            }

            Comp.OBLabel {
                id: headerLabel
                width: headerRow.width - previousButton.width - nextButton.width - 2 * headerRow.spacing
                // Match the buttons' height rather than the text's own thin
                // implicit height, so the whole row height is tappable, not
                // just a narrow vertically-centered band.
                height: previousButton.height
                text: root.headerText
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter

                MouseArea {
                    anchors.fill: parent
                    onClicked: root.zoomOut()
                }
            }

            Comp.OBButton {
                id: nextButton
                text: "›"
                implicitWidth: root.cellSize
                anchors.verticalCenter: parent.verticalCenter
                onClicked: root.goToNext()
            }
        }

        StackView {
            id: calendarStack
            // StackView does not inherit an implicit size from pushed items,
            // so without this it collapses to 0x0 and nothing is visible.
            width: calendarStack.currentItem ? calendarStack.currentItem.implicitWidth : root.daysGridWidth
            height: calendarStack.currentItem ? calendarStack.currentItem.implicitHeight : root.daysGridHeight
            initialItem: daysViewComponent
            // No clip: the push/pop transitions below intentionally
            // overshoot the item's normal bounds mid-animation.

            // Zooming out (push) grows the incoming view down to size, as
            // if the view is pulling back to reveal the broader unit;
            // zooming in (pop) shrinks the incoming view up to size, as if
            // pushing back into the finer-grained one.
            pushEnter: Transition {
                ParallelAnimation {
                    NumberAnimation { property: "scale"; from: 1.3; to: 1.0; duration: 200; easing.type: Easing.OutCubic }
                    NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 200 }
                }
            }
            pushExit: Transition {
                NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 200 }
            }
            popEnter: Transition {
                ParallelAnimation {
                    NumberAnimation { property: "scale"; from: 0.7; to: 1.0; duration: 200; easing.type: Easing.OutCubic }
                    NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 200 }
                }
            }
            popExit: Transition {
                NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 200 }
            }
        }
    }

    Component {
        id: daysViewComponent

        Column {
            spacing: 2

            Row {
                spacing: 2

                Item {
                    width: weekNumberColumn.width
                    height: dayOfWeekRow.height
                }

                DayOfWeekRow {
                    id: dayOfWeekRow
                    locale: monthGrid.locale
                    width: monthGrid.width

                    delegate: Comp.OBHeader {
                        required property var model
                        text: model.shortName
                        pixelSize: 12
                        implicitWidth: root.cellSize
                        implicitHeight: root.cellSize
                        leftPadding: 2
                        rightPadding: 2
                        topPadding: 2
                        bottomPadding: 2
                    }
                }
            }

            Row {
                spacing: 2

                WeekNumberColumn {
                    id: weekNumberColumn
                    month: monthGrid.month
                    year: monthGrid.year
                    locale: monthGrid.locale
                    width: root.weekColumnWidth
                    height: monthGrid.height

                    delegate: Comp.OBHeader {
                        required property int weekNumber
                        text: String(weekNumber)
                        pixelSize: 12
                        orientation: Gradient.Horizontal
                        implicitWidth: root.cellSize
                        implicitHeight: root.cellSize
                        leftPadding: 2
                        rightPadding: 2
                        topPadding: 2
                        bottomPadding: 2
                    }
                }

                MonthGrid {
                    id: monthGrid
                    month: root.displayedMonth - 1
                    year: root.displayedYear
                    locale: Qt.locale()
                    width: root.cellSize * 7
                    height: root.cellSize * 6

                    delegate: Rectangle {
                        id: dayCell
                        required property var model
                        readonly property bool selected: root.isDateSelected(dayCell.model.date)
                        readonly property bool inCurrentMonth: dayCell.model.month === monthGrid.month

                        implicitWidth: root.cellSize
                        implicitHeight: root.cellSize
                        gradient: dayCell.selected ? Style.OBStyle.selected : Style.OBStyle.background
                        opacity: dayCell.inCurrentMonth ? 1.0 : 0.4

                        Comp.OBLabel {
                            anchors.centerIn: parent
                            text: dayCell.model.day
                        }

                        // Day-only total.
                        Rectangle {
                            visible: dayCell.inCurrentMonth && root.dailyTotal !== null
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 3
                            color: root.dailyTotal && root.dailyTotal(dayCell.model.date) >= 0 ?
                                       Style.OBTheme.palette.success : Style.OBTheme.palette.danger
                        }

                        // Running total from the period start through this day.
                        Rectangle {
                            visible: dayCell.inCurrentMonth && root.cumulativeTotal !== null
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 3
                            color: root.cumulativeTotal && root.cumulativeTotal(dayCell.model.date) >= 0 ?
                                       Style.OBTheme.palette.success : Style.OBTheme.palette.danger
                        }

                        MouseArea {
                            enabled: dayCell.inCurrentMonth
                            anchors.fill: parent
                            onClicked: (mouse) => root.handleDayTapped(dayCell.model.date, (mouse.modifiers & Qt.ShiftModifier) !== 0)
                        }
                    }
                }
            }
        }
    }

    Component {
        id: monthsViewComponent

        Grid {
            columns: root.pickerColumns
            rowSpacing: 2
            columnSpacing: 2

            Repeater {
                model: 12
                delegate: Rectangle {
                    id: monthCell
                    required property int index
                    readonly property bool isDisplayed: (monthCell.index + 1) === root.displayedMonth

                    width: root.pickerCellWidth
                    height: root.pickerCellHeight
                    gradient: monthCell.isDisplayed ? Style.OBStyle.selected : Style.OBStyle.background

                    Comp.OBLabel {
                        anchors.centerIn: parent
                        text: Qt.locale().monthName(monthCell.index, Locale.ShortFormat)
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: root.handleMonthTapped(monthCell.index + 1)
                    }
                }
            }
        }
    }

    Component {
        id: yearsViewComponent

        Grid {
            columns: root.pickerColumns
            rowSpacing: 2
            columnSpacing: 2

            Repeater {
                model: 12
                delegate: Rectangle {
                    id: yearCell
                    required property int index
                    readonly property int year: root.decadeStart - 1 + yearCell.index
                    readonly property bool inDecade: yearCell.year >= root.decadeStart && yearCell.year <= root.decadeStart + 9
                    readonly property bool isDisplayed: yearCell.year === root.displayedYear

                    width: root.pickerCellWidth
                    height: root.pickerCellHeight
                    gradient: yearCell.isDisplayed ? Style.OBStyle.selected : Style.OBStyle.background
                    opacity: yearCell.inDecade ? 1.0 : 0.4

                    Comp.OBLabel {
                        anchors.centerIn: parent
                        text: String(yearCell.year)
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: root.selectYear(yearCell.year)
                    }
                }
            }
        }
    }
}
