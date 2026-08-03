import QtQuick
import QtTest

import "../../QML/ComposedComponent" as Composed
import "../../QML/Style" as Style

TestCase {
    id: testCase
    name: "TableRow"
    when: windowShown
    width: 300
    height: 100

    Composed.TableRow {
        id: row
        text: "Groceries"
    }

    function test_defaultIsUnselected() {
        compare(row.selected, false)
    }

    // TableRow reacts to a bare TapHandler, and mouseClick() synthetic
    // events aren't reliably delivered under the offscreen QPA platform
    // used for headless CI. toggleSelected() is the exact function
    // TapHandler.onTapped invokes, so calling it directly still exercises
    // the real tap logic.
    function test_tapTogglesSelected() {
        row.toggleSelected()
        compare(row.selected, true)
        row.toggleSelected()
        compare(row.selected, false)
    }

    function test_selectAndDeselect() {
        row.select()
        compare(row.selected, true)
        row.deselect()
        compare(row.selected, false)
    }

    function test_defaultRowStateIsNeutral() {
        compare(row.rowState, Composed.TableRow.RowState.Neutral)
        compare(row.highlightGradient, row.selectedGradient)
    }

    function test_positiveRowStateUsesPositiveGradient() {
        row.rowState = Composed.TableRow.RowState.Positive
        compare(row.highlightGradient, row.positiveGradient)
        row.rowState = Composed.TableRow.RowState.Neutral
    }

    function test_negativeRowStateUsesNegativeGradient() {
        row.rowState = Composed.TableRow.RowState.Negative
        compare(row.highlightGradient, row.negativeGradient)
        row.rowState = Composed.TableRow.RowState.Neutral
    }
}
