import QtQuick

import OpenAccount.Model

import "../Style" as Style
import "../Components" as Comp

// Cross-budget "at a glance" summary: for each date in `dates` (defaults to
// today), lists every Budget whose period covers it — name, that instance's
// end date, its current spent amount, and usage percentage — with the row's
// text (not a background wash) colored Over/Within. Unlike OBBudgetPage's
// Details table (one budget, one instance, full transaction list), this
// only ever shows one summary line per (budget, date) match: read-only, not
// selectable/tappable.
//
// State is derived the same way OBBudgetPage computes it: the sum of
// transactionModel's rows matching the budget's category within that
// specific period instance, compared against its threshold — not
// Budget.overBudget()/usedAmount(), which stay 0/false unless something
// calls the non-invokable Budget.addTransaction().
Item {
    id: root

    property BudgetListModel budgetModel: null
    property AccountTransactionFilterProxyModel transactionModel: null
    property var dates: [new Date()]

    // Name + End + Used + % columns, their spacing/margins, sized to match
    // the Row below.
    implicitWidth: Style.OBConstants.widthMedium + Style.OBConstants.widthLittle * 2 +
                   Style.OBConstants.widthLittle / 2 + Style.OBConstants.horizontalSpacing * 3 +
                   Style.OBConstants.leftMargins + Style.OBConstants.rightMargins
    implicitHeight: entriesColumn.implicitHeight

    // One entry per (budget, date) pair where the date falls inside one of
    // that budget's period instances: its base [startDate, endDate],
    // stepped forward by recurrence up to 4 times (same cap OBBudgetPage's
    // Instance column uses). A budget matching several of `dates` within
    // the same instance only contributes one entry for it.
    readonly property var matchingEntries: {
        const entries = []
        if (!root.budgetModel) return entries

        for (let b = 0; b < root.budgetModel.count; ++b) {
            const budget = root.budgetModel.at(b)
            if (!budget || isNaN(budget.startDate.getTime()) || isNaN(budget.endDate.getTime()))
                continue

            const stepDays = budget.recurrence > 0 ? budget.recurrence : 0
            const instanceCount = stepDays > 0 ? 4 : 1
            let start = new Date(budget.startDate)
            let end = new Date(budget.endDate)

            for (let i = 0; i < instanceCount; ++i) {
                for (let d = 0; d < root.dates.length; ++d) {
                    if (root.dates[d].getTime() >= start.getTime() && root.dates[d].getTime() <= end.getTime()) {
                        entries.push({ budget: budget, start: new Date(start), end: new Date(end) })
                        break
                    }
                }
                start.setDate(start.getDate() + stepDays)
                end.setDate(end.getDate() + stepDays)
            }
        }
        return entries
    }

    Column {
        id: entriesColumn
        width: parent.width
        spacing: Style.OBConstants.verticalSpacing / 2

        Row {
            visible: root.matchingEntries.length > 0
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Style.OBConstants.leftMargins
            anchors.rightMargin: Style.OBConstants.rightMargins
            spacing: Style.OBConstants.horizontalSpacing

            Comp.OBLabel { width: Style.OBConstants.widthMedium; text: qsTr("Budget"); font.bold: true }
            Comp.OBLabel { width: Style.OBConstants.widthLittle; text: qsTr("End"); font.bold: true }
            Comp.OBLabel { width: Style.OBConstants.widthLittle; text: qsTr("Used"); font.bold: true; horizontalAlignment: Text.AlignRight }
            Comp.OBLabel { width: Style.OBConstants.widthLittle / 2; text: qsTr("%"); font.bold: true; horizontalAlignment: Text.AlignRight }
        }

        Repeater {
            model: root.matchingEntries

            delegate: Item {
                id: entryRoot
                // Lets ViewTests/qml/tst_OBBudgetToday.qml reach a rendered
                // row's computed usedAmount/usageRatio/overBudget via
                // TestCase.findChild(); matchingEntries only carries the
                // budget/period each row is scoped to, not the state
                // computed from it.
                objectName: "budgetTodayEntry"
                width: entriesColumn.width
                height: Style.OBConstants.heightMedium

                // Scoped to this one entry's budget + period instance, same
                // filters OBBudgetPage's Details table applies for the
                // selected budget/instance.
                AccountTransactionFilterProxyModel {
                    id: entryProxy
                    sourceModel: root.transactionModel
                    categoryFilter: [modelData.budget.categoryId]
                    dateFrom: modelData.start
                    dateTo: modelData.end
                }

                readonly property real usedAmount: {
                    let total = 0
                    for (let i = 0; i < entryProxy.count; ++i)
                        total += Math.abs(entryProxy.get(i).value)
                    return total
                }
                readonly property real usageRatio: modelData.budget.threshold > 0 ?
                                                    (entryRoot.usedAmount / modelData.budget.threshold * 100) : 0
                readonly property bool overBudget: entryRoot.usedAmount > modelData.budget.threshold
                // Drives every label's color below instead of a row
                // background wash: Negative past the threshold (ratio over
                // 100%), Positive under it.
                readonly property int rowTextState: entryRoot.overBudget ? Comp.OBLabel.TextState.Negative : Comp.OBLabel.TextState.Positive

                Row {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: Style.OBConstants.leftMargins
                    anchors.rightMargin: Style.OBConstants.rightMargins
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: Style.OBConstants.horizontalSpacing

                    Comp.OBLabel { width: Style.OBConstants.widthMedium; text: modelData.budget.name; textState: entryRoot.rowTextState }
                    Comp.OBLabel { width: Style.OBConstants.widthLittle; text: Qt.formatDate(modelData.end, "dd/MM/yy"); textState: entryRoot.rowTextState }
                    Comp.OBLabel { width: Style.OBConstants.widthLittle; text: entryRoot.usedAmount.toFixed(2); horizontalAlignment: Text.AlignRight; textState: entryRoot.rowTextState }
                    Comp.OBLabel { width: Style.OBConstants.widthLittle / 2; text: entryRoot.usageRatio.toFixed(0) + "%"; horizontalAlignment: Text.AlignRight; textState: entryRoot.rowTextState }
                }
            }
        }

        Comp.OBLabel {
            width: entriesColumn.width
            height: Style.OBConstants.heightMedium
            visible: root.matchingEntries.length === 0
            text: qsTr("No budget for the selected date(s)")
            textState: Comp.OBLabel.TextState.Disabled
        }
    }
}
