import QtQuick
import QtTest

import "../../QML/Components" as Comp

TestCase {
    id: testCase
    name: "OBRadioButton"
    when: windowShown
    width: 200
    height: 100

    Comp.OBRadioButton {
        id: radio
        text: "Option A"
    }

    function init() {
        radio.checked = false
    }

    // mouseClick() synthetic events aren't reliably delivered under the
    // offscreen QPA platform used for headless CI; keyboard activation
    // (Space) exercises the same AbstractButton click path and doubles
    // as a keyboard-accessibility check.
    function test_clickTogglesChecked() {
        radio.forceActiveFocus()
        keyClick(Qt.Key_Space)
        compare(radio.checked, true)
    }

    function test_indicatorSizeMatchesConstant() {
        compare(radio.indicator.width, radio.indicatorSize)
        compare(radio.indicator.height, radio.indicatorSize)
    }

    function test_indicatorIsCircular() {
        compare(radio.indicator.radius, radio.indicator.width / 2)
    }
}
