import QtQuick
import QtTest

import "../../QML/Components" as Comp

TestCase {
    id: testCase
    name: "OBSpinBox"
    when: windowShown
    width: 300
    height: 200

    Comp.OBSpinBox {
        id: spin
        decimals: 1
        realFrom: 0
        realTo: 10
        realStepSize: 0.5
    }

    function init() {
        spin.realValue = 2
    }

    function test_realValueScalesInternalValue() {
        compare(spin.decimalFactor, 10)
        compare(spin.value, 20)
    }

    function test_settingRealValueUpdatesValue() {
        spin.realValue = 3.5
        compare(spin.value, 35)
    }

    function test_settingValueUpdatesRealValue() {
        spin.value = 40
        compare(spin.realValue, 4)
    }

    // mouseClick() synthetic events aren't reliably delivered to the
    // up/down indicators under the offscreen QPA platform used for
    // headless CI; SpinBox natively increments/decrements on Up/Down
    // when focused, exercising the same code path.
    function test_upIndicatorIncrementsByStepSize() {
        spin.forceActiveFocus()
        keyClick(Qt.Key_Up)
        fuzzyCompare(spin.realValue, 2.5, 0.001)
    }

    function test_downIndicatorDecrementsByStepSize() {
        spin.forceActiveFocus()
        keyClick(Qt.Key_Down)
        fuzzyCompare(spin.realValue, 1.5, 0.001)
    }

    function test_valueFromTextRoundTripsWithTextFromValue() {
        // textFromValue()/valueFromText() must agree on the same locale
        // (e.g. "3.5" is not valid input under a locale using "," as the
        // decimal separator), so round-trip through textFromValue() rather
        // than hardcoding a "."-formatted string.
        var text = spin.textFromValue(35, spin.locale)
        var v = spin.valueFromText(text, spin.locale)
        compare(v, 35)
    }
}
