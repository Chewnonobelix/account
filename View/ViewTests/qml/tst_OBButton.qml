import QtQuick
import QtTest

import "../../QML/Components" as Comp
import "../../QML/Style" as Style

TestCase {
    id: testCase
    name: "OBButton"
    when: windowShown
    width: 300
    height: 200

    Comp.OBButton {
        id: button
        text: "Click me"
    }

    SignalSpy {
        id: clickSpy
        target: button
        signalName: "clicked"
    }

    function init() {
        clickSpy.clear()
        button.enabled = true
        button.text = "Click me"
    }

    function test_defaultSize() {
        compare(button.implicitWidth, Style.OBConstants.widthMedium)
        compare(button.implicitHeight, Style.OBConstants.heightMedium)
    }

    function test_clickEmitsClicked() {
        // mouseClick() synthetic events aren't reliably delivered under the
        // offscreen QPA platform used for headless CI; keyboard activation
        // (Space) exercises the same AbstractButton click path and doubles
        // as a keyboard-accessibility check.
        button.forceActiveFocus()
        keyClick(Qt.Key_Space)
        compare(clickSpy.count, 1)
    }

    function test_textReflectsInContentLabel() {
        button.text = "Save"
        compare(button.contentItem.text, "Save")
    }

    function test_disabledUsesDisabledBorderColor() {
        button.enabled = false
        compare(button.background.border.color, button.borderColorDisabled)
    }

    function test_enabledUsesNormalBorderColor() {
        compare(button.background.border.color, button.borderColorNormal)
    }
}
