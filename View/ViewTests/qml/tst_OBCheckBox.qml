import QtQuick
import QtTest

import "../../QML/Components" as Comp

TestCase {
    id: testCase
    name: "OBCheckBox"
    when: windowShown
    width: 200
    height: 100

    Comp.OBCheckBox {
        id: check
        text: "Accept"
    }

    function init() {
        check.tristate = false
        check.checkState = Qt.Unchecked
    }

    // mouseClick() synthetic events aren't reliably delivered under the
    // offscreen QPA platform used for headless CI; keyboard activation
    // (Space) exercises the same AbstractButton click path and doubles
    // as a keyboard-accessibility check.
    function test_clickTogglesChecked() {
        check.forceActiveFocus()
        keyClick(Qt.Key_Space)
        compare(check.checked, true)
        keyClick(Qt.Key_Space)
        compare(check.checked, false)
    }

    function test_tristateCyclesThroughThreeStates() {
        check.tristate = true
        check.forceActiveFocus()
        keyClick(Qt.Key_Space)
        compare(check.checkState, Qt.PartiallyChecked)
        keyClick(Qt.Key_Space)
        compare(check.checkState, Qt.Checked)
        keyClick(Qt.Key_Space)
        compare(check.checkState, Qt.Unchecked)
    }

    function test_indicatorSizeMatchesConstant() {
        compare(check.indicator.width, check.indicatorSize)
        compare(check.indicator.height, check.indicatorSize)
    }
}
