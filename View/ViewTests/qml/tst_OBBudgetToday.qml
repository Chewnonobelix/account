import QtQuick
import QtTest

import OpenAccount.Model

import "../../QML/ComposedComponent" as Composed

TestCase {
    id: testCase
    name: "OBBudgetToday"
    when: windowShown
    width: 400
    height: 300

    Component { id: budgetPrototype; Budget {} }
    Component { id: transactionPrototype; Transaction {} }
    Component { id: budgetModelComponent; BudgetListModel {} }
    Component { id: transactionModelComponent; TransactionListModel {} }
    Component { id: proxyModelComponent; AccountTransactionFilterProxyModel {} }
    Component { id: widgetComponent; Composed.OBBudgetToday {} }

    function makeBudget(model, name, categoryId, threshold, start, end, recurrence) {
        const budget = budgetPrototype.createObject(model, {
            name: name, categoryId: categoryId, threshold: threshold,
            startDate: start, endDate: end, recurrence: recurrence
        })
        model.addBudget(budget)
        return budget
    }

    function makeTransaction(model, categoryId, value, date) {
        const tx = transactionPrototype.createObject(model, {
            value: value, movement: OpenAccountEnums.Movement.Debit,
            date: date, category: categoryId
        })
        model.addTransaction(tx)
        return tx
    }

    function test_defaultDatesIsToday() {
        const budgetModel = budgetModelComponent.createObject(testCase)
        const widget = widgetComponent.createObject(testCase, { budgetModel: budgetModel })

        compare(widget.dates.length, 1)
        const today = new Date()
        verify(widget.dates[0].getFullYear() === today.getFullYear() &&
               widget.dates[0].getMonth() === today.getMonth() &&
               widget.dates[0].getDate() === today.getDate())

        widget.destroy()
        budgetModel.destroy()
    }

    function test_noBudgets_hasNoMatchingEntries() {
        const budgetModel = budgetModelComponent.createObject(testCase)
        const widget = widgetComponent.createObject(testCase, { budgetModel: budgetModel })

        compare(widget.matchingEntries.length, 0)

        widget.destroy()
        budgetModel.destroy()
    }

    function test_budgetCoveringDate_isIncluded() {
        const budgetModel = budgetModelComponent.createObject(testCase)
        makeBudget(budgetModel, "Groceries", "{11111111-0000-0000-0000-000000000001}", 100,
                   new Date(2026, 0, 1), new Date(2026, 0, 31), OpenAccountEnums.Frequency.Once)

        const widget = widgetComponent.createObject(testCase, {
            budgetModel: budgetModel, dates: [new Date(2026, 0, 15)]
        })

        compare(widget.matchingEntries.length, 1)
        compare(widget.matchingEntries[0].budget.name, "Groceries")

        widget.destroy()
        budgetModel.destroy()
    }

    function test_budgetNotCoveringDate_isExcluded() {
        const budgetModel = budgetModelComponent.createObject(testCase)
        makeBudget(budgetModel, "Groceries", "{11111111-0000-0000-0000-000000000002}", 100,
                   new Date(2026, 0, 1), new Date(2026, 0, 31), OpenAccountEnums.Frequency.Once)

        const widget = widgetComponent.createObject(testCase, {
            budgetModel: budgetModel, dates: [new Date(2026, 1, 15)]
        })

        compare(widget.matchingEntries.length, 0)

        widget.destroy()
        budgetModel.destroy()
    }

    function test_recurringBudget_matchesSteppedInstance() {
        const budgetModel = budgetModelComponent.createObject(testCase)
        // Weekly, starting 01/01: the 3rd instance covers 15/01-21/01.
        makeBudget(budgetModel, "Weekly groceries", "{11111111-0000-0000-0000-000000000003}", 100,
                   new Date(2026, 0, 1), new Date(2026, 0, 7), OpenAccountEnums.Frequency.Weekly)

        const widget = widgetComponent.createObject(testCase, {
            budgetModel: budgetModel, dates: [new Date(2026, 0, 17)]
        })

        compare(widget.matchingEntries.length, 1)

        widget.destroy()
        budgetModel.destroy()
    }

    function test_usageUnderThreshold_showsWithinBudgetState() {
        const categoryId = "{11111111-0000-0000-0000-000000000004}"
        const budgetModel = budgetModelComponent.createObject(testCase)
        makeBudget(budgetModel, "Groceries", categoryId, 100,
                   new Date(2026, 0, 1), new Date(2026, 0, 31), OpenAccountEnums.Frequency.Once)

        const transactionModel = transactionModelComponent.createObject(testCase)
        makeTransaction(transactionModel, categoryId, 40, new Date(2026, 0, 15))
        const proxy = proxyModelComponent.createObject(testCase, { sourceModel: transactionModel })

        const widget = widgetComponent.createObject(testCase, {
            budgetModel: budgetModel, transactionModel: proxy, dates: [new Date(2026, 0, 15)]
        })

        const entry = findChild(widget, "budgetTodayEntry")
        verify(entry !== null)
        compare(entry.usedAmount, 40)
        compare(entry.usageRatio, 40)
        compare(entry.overBudget, false)

        widget.destroy()
        proxy.destroy()
        transactionModel.destroy()
        budgetModel.destroy()
    }

    function test_usageOverThreshold_showsOverBudgetState() {
        const categoryId = "{11111111-0000-0000-0000-000000000005}"
        const budgetModel = budgetModelComponent.createObject(testCase)
        makeBudget(budgetModel, "Groceries", categoryId, 30,
                   new Date(2026, 0, 1), new Date(2026, 0, 31), OpenAccountEnums.Frequency.Once)

        const transactionModel = transactionModelComponent.createObject(testCase)
        makeTransaction(transactionModel, categoryId, 45, new Date(2026, 0, 15))
        const proxy = proxyModelComponent.createObject(testCase, { sourceModel: transactionModel })

        const widget = widgetComponent.createObject(testCase, {
            budgetModel: budgetModel, transactionModel: proxy, dates: [new Date(2026, 0, 15)]
        })

        const entry = findChild(widget, "budgetTodayEntry")
        verify(entry !== null)
        compare(entry.usedAmount, 45)
        fuzzyCompare(entry.usageRatio, 150, 0.01)
        compare(entry.overBudget, true)

        widget.destroy()
        proxy.destroy()
        transactionModel.destroy()
        budgetModel.destroy()
    }
}
