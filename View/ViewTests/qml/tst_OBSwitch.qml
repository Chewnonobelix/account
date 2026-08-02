import QtQuick
import QtTest

import "../../QML/Components" as Comp

TestCase {
    id: testCase
    name: "OBSwitch"
    when: windowShown
    width: 200
    height: 100

    Comp.OBSwitch {
        id: sw
    }

    function init() {
        sw.tristate = false
        sw.checkState = Qt.Unchecked
    }

    function test_defaultIsUnchecked() {
        compare(sw.checkState, Qt.Unchecked)
        compare(sw.checked, false)
    }

    // OBSwitch reacts to a bare TapHandler (no keyboard binding), and
    // mouseClick() synthetic events aren't reliably delivered under the
    // offscreen QPA platform used for headless CI. nextCheckState() is the
    // exact function TapHandler.onTapped invokes, so calling it directly
    // still exercises the real state-cycling logic behind a tap.
    function test_binaryTapTogglesCheckedState() {
        sw.nextCheckState()
        compare(sw.checkState, Qt.Checked)
        compare(sw.checked, true)
        sw.nextCheckState()
        compare(sw.checkState, Qt.Unchecked)
        compare(sw.checked, false)
    }

    function test_tristateCyclesThroughThreeStates() {
        sw.tristate = true
        sw.nextCheckState()
        compare(sw.checkState, Qt.PartiallyChecked)
        sw.nextCheckState()
        compare(sw.checkState, Qt.Checked)
        sw.nextCheckState()
        compare(sw.checkState, Qt.Unchecked)
    }

    function test_checkedPropertyStaysInSyncWhenNotTristate() {
        sw.checked = true
        compare(sw.checkState, Qt.Checked)
    }
}
