import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import OpenAccount.Model

import "../Style" as Style
import "../Components" as Comp
import "../ComposedComponent" as Composed

// Budget management page: a Budget list, the selected budget's generated
// period instances, an editable configuration panel, a summary panel, and
// a paginated table of the transactions matched against it.
//
// Presentational only, same rule as the Popup/ components: it edits the
// selectedBudget object in place (mirroring OBTransactionInfo's editable
// fields) but doesn't own persistence, and iterationNumber/overedCount/
// respectedCount aren't derivable from a single Budget (no per-period
// history is tracked yet), so the caller supplies them.
Item {
    id: root

    property BudgetListModel budgetModel: null
    property CategoryListModel categoryModel: null
    property AccountTransactionFilterProxyModel transactionModel: null

    property int iterationNumber: 0
    property int overedCount: 0
    property int respectedCount: 0
    readonly property int overedPercent: root.iterationNumber > 0 ? Math.round(100 * root.overedCount / root.iterationNumber) : 0
    readonly property int respectedPercent: root.iterationNumber > 0 ? Math.round(100 * root.respectedCount / root.iterationNumber) : 0

    property alias selectedIndex: budgetListView.currentIndex
    readonly property Budget selectedBudget: root.budgetModel && root.selectedIndex >= 0 ?
                                              root.budgetModel.at(root.selectedIndex) : null

    // State of the selected Instance row (used amount / remaining / ratio),
    // computed straight from budgetTransactionProxy — the same
    // category+period filtered set of transactions shown in the Details
    // table below — rather than from Budget's own usedAmount(), which only
    // ever reflects the budget's base [startDate, endDate] period (not
    // whichever stepped instance is currently selected), and stays 0 anyway
    // since nothing here calls the non-invokable Budget.addTransaction()
    // (see file header).
    readonly property real instanceUsedAmount: {
        if (!root.selectedBudget) return 0
        let total = 0
        for (let i = 0; i < budgetTransactionProxy.count; ++i)
            total += Math.abs(budgetTransactionProxy.get(i).value)
        return total
    }
    readonly property real instanceRemaining: root.selectedBudget ? root.selectedBudget.threshold - root.instanceUsedAmount : 0
    readonly property real instanceUsageRatio: root.selectedBudget && root.selectedBudget.threshold > 0 ?
                                                (root.instanceUsedAmount / root.selectedBudget.threshold * 100) : 0

    // Mirrors the Figma "state=good" / "state=bad" symbols: driven by
    // whether the selected instance is over its threshold, not a mode toggle.
    readonly property bool overBudget: root.selectedBudget !== null && root.instanceUsedAmount > root.selectedBudget.threshold

    // Configuration panel drives the top row's height: header + 5 DescLine
    // rows + the gaps/margins around them.
    readonly property real topRowHeight: Style.OBConstants.heightBig + Style.OBConstants.heightMedium * 5 +
                                          Style.OBConstants.verticalSpacing * 4 + Style.OBConstants.leftMargins * 2

    implicitWidth: Style.OBConstants.widthBig * 8
    implicitHeight: root.topRowHeight + Style.OBConstants.verticalSpacing + Style.OBConstants.heightMedium * 8

    FrequencyModel {
        id: frequencyModel
    }

    // Narrows root.transactionModel (already account-scoped by the caller,
    // e.g. UiKit's shared accountTransactionProxy) down to exactly the
    // selected budget's category and the selected Instance row's period, so
    // the Details table below only ever shows transactions that actually
    // belong to what's currently picked in the Budget/Instance columns.
    AccountTransactionFilterProxyModel {
        id: budgetTransactionProxy
        sourceModel: root.transactionModel
        categoryFilter: root.selectedBudget ? [root.selectedBudget.categoryId] : []
        dateFrom: {
            const range = root.selectedInstanceRange()
            return range ? range.start : new Date(NaN)
        }
        dateTo: {
            const range = root.selectedInstanceRange()
            return range ? range.end : new Date(NaN)
        }
    }

    // Real [start, end] Date pairs behind the Instance column. Budget only
    // stores one [startDate, endDate] period; for a repeatable budget
    // (recurrence's numeric value is its day count, per OpenAccountEnums)
    // this steps that period forward to produce a handful of upcoming
    // instances, capped at 4 to match the design. instancePeriods() (the
    // ListView's display model) and the Details table's date-range filter
    // both derive from this so they can never disagree on where an
    // instance's period actually falls.
    function instanceRanges() {
        const budget = root.selectedBudget
        if (!budget || isNaN(budget.startDate.getTime()) || isNaN(budget.endDate.getTime())) return []

        const stepDays = budget.recurrence > 0 ? budget.recurrence : 0
        const count = stepDays > 0 ? 4 : 1
        const ranges = []
        let start = new Date(budget.startDate)
        let end = new Date(budget.endDate)
        for (let i = 0; i < count; ++i) {
            ranges.push({ start: new Date(start), end: new Date(end) })
            start.setDate(start.getDate() + stepDays)
            end.setDate(end.getDate() + stepDays)
        }
        return ranges
    }

    // Display-only period labels for the Instance column.
    function instancePeriods() {
        return root.instanceRanges().map(function (range) {
            return Qt.formatDate(range.start, "dd/MM/yy") + " - " + Qt.formatDate(range.end, "dd/MM/yy")
        })
    }

    // [start, end] of whichever Instance row is selected, defaulting to the
    // first (current) period when nothing's been explicitly tapped yet —
    // the Details table below should have something meaningful to show as
    // soon as a budget is picked, not require a second click on Instance.
    function selectedInstanceRange() {
        const ranges = root.instanceRanges()
        if (ranges.length === 0) return null

        const index = Math.max(0, Math.min(instanceListView.currentIndex, ranges.length - 1))
        return ranges[index]
    }

    function durationLabel() {
        const budget = root.selectedBudget
        if (!budget || isNaN(budget.startDate.getTime()) || isNaN(budget.endDate.getTime())) return ""
        const days = Math.round((budget.endDate.getTime() - budget.startDate.getTime()) / 86400000)
        return days + " " + qsTr("days")
    }

    // Selectable row for the Budget/Instance columns. Unlike TableRow
    // (which toggles its own `selected` on tap, clobbering any external
    // binding), selection here is owned by the ListView's currentIndex, so
    // a tap only ever assigns that index.
    component ListRow: Item {
        id: rowRoot
        property string text: ""
        property bool selected: false
        // Neutral (grey) selection for the Budget column; the Instance
        // column is the one that carries the good/bad wash, per the design.
        property bool neutral: false
        property bool bad: false
        implicitWidth: Style.OBConstants.widthMedium
        implicitHeight: Style.OBConstants.heightMedium

        Rectangle {
            anchors.fill: parent
            visible: rowRoot.selected
            gradient: rowRoot.neutral ? Style.OBStyle.selected :
                      (rowRoot.bad ? Style.OBStyle.negative : Style.OBStyle.positive)
        }
        Comp.OBLabel {
            text: rowRoot.text
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Style.OBConstants.leftMargins
            anchors.rightMargin: Style.OBConstants.rightMargins
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    // Reused by the Budget/Instance/Configuration/Summary panels: a header
    // banner over a gold-bordered card, sized via Layout attached properties
    // on the instance instead of anchors so it drops straight into the
    // panels' RowLayout.
    component Panel: ColumnLayout {
        id: panelRoot
        property string title: ""
        default property alias content: body.data

        spacing: 0

        Comp.OBHeader {
            text: panelRoot.title
            Layout.fillWidth: true
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: Style.OBConstants.borderRadius
            border.width: Style.OBConstants.borderWidth
            border.color: Style.OBTheme.palette.outline
            gradient: Style.OBStyle.background
            clip: true

            Item {
                id: body
                anchors.fill: parent
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: Style.OBConstants.verticalSpacing

        // Budget/Instance run the full page height, unlike Configuration/
        // Summary/Details which only occupy the right-hand column below.
        Panel {
            title: qsTr("Budget")
            Layout.preferredWidth: Style.OBConstants.widthMedium
            Layout.fillHeight: true

            ListView {
                id: budgetListView
                anchors.fill: parent
                clip: true
                model: root.budgetModel

                ScrollBar.vertical: Comp.OBScrollBar {}

                delegate: ListRow {
                    width: budgetListView.width
                    text: model.name
                    selected: index === budgetListView.currentIndex
                    neutral: true

                    TapHandler {
                        onTapped: budgetListView.currentIndex = index
                    }
                }
            }
        }

        Panel {
            title: qsTr("Instance")
            Layout.preferredWidth: Style.OBConstants.widthBig
            Layout.fillHeight: true

            ListView {
                id: instanceListView
                anchors.fill: parent
                clip: true
                model: root.instancePeriods()

                ScrollBar.vertical: Comp.OBScrollBar {}

                delegate: ListRow {
                    width: instanceListView.width
                    text: modelData
                    selected: index === instanceListView.currentIndex
                    bad: root.overBudget

                    TapHandler {
                        onTapped: instanceListView.currentIndex = index
                    }
                }
            }
        }

        // Configuration + Summary + Details share this column's width, so
        // Details naturally ends up exactly as wide as the two panels above
        // it instead of stretching under Budget/Instance too.
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: Style.OBConstants.verticalSpacing

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: root.topRowHeight
                spacing: Style.OBConstants.verticalSpacing

            Panel {
                title: qsTr("Configuration")
                Layout.preferredWidth: Style.OBConstants.widthBig * 4
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Style.OBConstants.leftMargins
                    spacing: Style.OBConstants.verticalSpacing

                    Composed.DescLine {
                        title: qsTr("Name (Category)")
                        Layout.fillWidth: true

                        Comp.OBComboBox {
                            Layout.fillWidth: true
                            model: root.categoryModel
                            textRole: "name"
                            valueRole: "id"
                            enabled: root.selectedBudget !== null && root.categoryModel !== null
                            currentIndex: root.selectedBudget && root.categoryModel ?
                                          root.categoryModel.indexOf(root.selectedBudget.categoryId) : -1
                            onActivated: function (index) {
                                if (!root.selectedBudget || !root.categoryModel) return
                                const category = root.categoryModel.at(index)
                                if (category) root.selectedBudget.categoryId = category.id
                            }
                        }
                    }

                    Composed.DescLine {
                        title: qsTr("Target")
                        Layout.fillWidth: true

                        RowLayout {
                            spacing: Style.OBConstants.horizontalSpacing

                            Comp.OBSpinBox {
                                decimals: 2
                                realFrom: 0
                                realTo: 1000000
                                enabled: root.selectedBudget !== null
                                realValue: root.selectedBudget ? root.selectedBudget.threshold : 0
                                onRealValueChanged: if (root.selectedBudget) root.selectedBudget.threshold = realValue
                            }

                            // No backing Budget field yet — kept as a plain
                            // visual toggle to match the design.
                            Comp.OBSwitch {
                                id: outcomeSwitch
                            }
                            Comp.OBLabel {
                                text: qsTr("Outcome")
                                Layout.alignment: Qt.AlignVCenter
                            }
                        }
                    }

                    Composed.DescLine {
                        title: qsTr("Recurrence")
                        Layout.fillWidth: true

                        Comp.OBComboBox {
                            Layout.fillWidth: true
                            model: frequencyModel
                            enabled: root.selectedBudget !== null
                            currentIndex: root.selectedBudget ? frequencyModel.indexOfValue(root.selectedBudget.recurrence) : -1
                            onActivated: function (index) { if (root.selectedBudget) root.selectedBudget.recurrence = frequencyModel.valueAt(index) }
                        }
                    }

                    Composed.DescLine {
                        title: qsTr("Duration")
                        Layout.fillWidth: true

                        Comp.OBLabel {
                            text: root.durationLabel()
                        }
                    }

                    Composed.DescLine {
                        title: qsTr("Begin")
                        Layout.fillWidth: true

                        Composed.OBDateButton {
                            enabled: root.selectedBudget !== null
                            selectedDate: root.selectedBudget ? root.selectedBudget.startDate : new Date(NaN)
                            onSelectedDateChanged: if (root.selectedBudget) root.selectedBudget.startDate = selectedDate
                        }
                    }
                }
            }

            Panel {
                title: qsTr("Summary")
                Layout.preferredWidth: Style.OBConstants.widthBig * 1.5
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Style.OBConstants.leftMargins
                    spacing: Style.OBConstants.verticalSpacing

                    // State of whichever row is highlighted in the Budget/
                    // Instance columns — see root.overBudget/instanceUsedAmount,
                    // both computed from budgetTransactionProxy so this always
                    // agrees with what the Details table below is showing.
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: Style.OBConstants.horizontalSpacing
                        Comp.OBLabel { text: qsTr("State"); font.bold: true }
                        Comp.OBLabel {
                            text: root.selectedBudget ? (root.overBudget ? qsTr("Over budget") : qsTr("Within budget"))
                                                       : qsTr("No selection")
                            textState: root.selectedBudget === null ? Comp.OBLabel.TextState.Disabled :
                                       (root.overBudget ? Comp.OBLabel.TextState.Negative : Comp.OBLabel.TextState.Positive)
                        }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: Style.OBConstants.horizontalSpacing
                        Comp.OBLabel { text: qsTr("Used"); font.bold: true }
                        Comp.OBLabel {
                            text: root.selectedBudget ?
                                  root.instanceUsedAmount.toFixed(2) + " / " + root.selectedBudget.threshold.toFixed(2) +
                                  " (" + root.instanceUsageRatio.toFixed(0) + "%)" : "—"
                        }
                        Item { Layout.fillWidth: true }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: Style.OBConstants.horizontalSpacing
                        Comp.OBLabel { text: qsTr("Iteration number"); font.bold: true }
                        Comp.OBLabel { text: "" + root.iterationNumber }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: Style.OBConstants.horizontalSpacing
                        Comp.OBLabel { text: qsTr("Overed"); font.bold: true }
                        Comp.OBLabel {
                            text: root.overedCount + " (" + root.overedPercent + "%)"
                            textState: Comp.OBLabel.TextState.Negative
                        }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: Style.OBConstants.horizontalSpacing
                        Comp.OBLabel { text: qsTr("Respected"); font.bold: true }
                        Comp.OBLabel {
                            text: root.respectedCount + " (" + root.respectedPercent + "%)"
                            textState: Comp.OBLabel.TextState.Positive
                        }
                        Item { Layout.fillWidth: true }
                    }
                }
            }
        }

        Panel {
            title: qsTr("Details")
            Layout.fillWidth: true
            Layout.preferredHeight: Style.OBConstants.heightMedium * 8

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                // Column titles for the TransactionRow layout below —
                // widths mirror TransactionRow's own column widths so the
                // header lines up with the data.
                Row {
                    Layout.fillWidth: true
                    Layout.leftMargin: Style.OBConstants.leftMargins
                    Layout.rightMargin: Style.OBConstants.rightMargins
                    Layout.topMargin: Style.OBConstants.verticalSpacing / 2
                    Layout.bottomMargin: Style.OBConstants.verticalSpacing / 2
                    spacing: Style.OBConstants.horizontalSpacing

                    Comp.OBLabel { width: Style.OBConstants.widthLittle / 2; text: qsTr("+/-"); font.bold: true; horizontalAlignment: Text.AlignHCenter }
                    Comp.OBLabel { width: Style.OBConstants.widthMedium; text: qsTr("Name"); font.bold: true }
                    Comp.OBLabel { width: Style.OBConstants.widthLittle; text: qsTr("Data"); font.bold: true }
                    Comp.OBLabel { width: Style.OBConstants.widthBig; text: qsTr("Label"); font.bold: true }
                    Comp.OBLabel { width: Style.OBConstants.widthLittle; text: qsTr("Value"); font.bold: true; horizontalAlignment: Text.AlignRight }
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: Style.OBConstants.borderWidth
                    color: Style.OBTheme.palette.outline
                }

                Composed.OBTransactionList {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    // null (not budgetTransactionProxy) while nothing is
                    // selected: an empty categoryFilter/dateFrom/dateTo
                    // means "unrestricted" on the proxy, which would show
                    // every transaction instead of none.
                    model: root.selectedBudget ? budgetTransactionProxy : null
                }
            }
        }
        }
    }
}
