import QtQuick
import QtQuick.Layouts
import QtTest

import "../../QML/ComposedComponent" as Composed

TestCase {
    id: testCase
    name: "OBButtonPair"
    when: windowShown
    width: 300
    height: 100

    Composed.OBButtonPair {
        id: pair
    }

    SignalSpy {
        id: leftSpy
        target: pair
        signalName: "leftClicked"
    }

    SignalSpy {
        id: rightSpy
        target: pair
        signalName: "rightClicked"
    }

    function init() {
        leftSpy.clear()
        rightSpy.clear()
        pair.leftText = "-"
        pair.rightText = "+"
        pair.leftEnabled = true
        pair.rightEnabled = true
        pair.stretch = false
    }

    function test_defaultTexts() {
        compare(pair.leftButton.text, "-")
        compare(pair.rightButton.text, "+")
    }

    function test_customTextsReflectInButtons() {
        pair.leftText = "Cancel"
        pair.rightText = "Accept"
        compare(pair.leftButton.text, "Cancel")
        compare(pair.rightButton.text, "Accept")
    }

    function test_leftClickEmitsLeftClicked() {
        // mouseClick() synthetic events aren't reliably delivered under the
        // offscreen QPA platform used for headless CI; keyboard activation
        // (Space) exercises the same AbstractButton click path and doubles
        // as a keyboard-accessibility check.
        pair.leftButton.forceActiveFocus()
        keyClick(Qt.Key_Space)
        compare(leftSpy.count, 1)
        compare(rightSpy.count, 0)
    }

    function test_rightClickEmitsRightClicked() {
        pair.rightButton.forceActiveFocus()
        keyClick(Qt.Key_Space)
        compare(rightSpy.count, 1)
        compare(leftSpy.count, 0)
    }

    function test_leftEnabledTogglesLeftButton() {
        pair.leftEnabled = false
        compare(pair.leftButton.enabled, false)
        compare(pair.rightButton.enabled, true)
    }

    function test_rightEnabledTogglesRightButton() {
        pair.rightEnabled = false
        compare(pair.rightButton.enabled, false)
        compare(pair.leftButton.enabled, true)
    }

    function test_stretchExposesFillWidthOnBothButtons() {
        compare(pair.leftButton.Layout.fillWidth, false)
        compare(pair.rightButton.Layout.fillWidth, false)

        pair.stretch = true
        compare(pair.leftButton.Layout.fillWidth, true)
        compare(pair.rightButton.Layout.fillWidth, true)
    }
}
