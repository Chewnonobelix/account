import QtQuick
import QtTest

import "../../QML/Components" as Comp
import "../../QML/Style" as Style

TestCase {
    id: testCase
    name: "OBTextInput"
    when: windowShown
    width: 300
    height: 200

    Comp.OBTextInput {
        id: input
    }

    function init() {
        input.text = ""
        input.focus = false
    }

    function test_defaultSize() {
        compare(input.implicitWidth, Style.OBConstants.widthMedium)
        compare(input.implicitHeight, Style.OBConstants.heightMedium)
    }

    function test_typingUpdatesText() {
        input.forceActiveFocus()
        keyClick(Qt.Key_A)
        keyClick(Qt.Key_B)
        keyClick(Qt.Key_C)
        compare(input.text, "abc")
    }

    function test_focusUsesFocusedBorderColor() {
        input.forceActiveFocus()
        compare(input.background.border.color, input.borderColorFocused)
    }

    function test_blurredUsesNormalBorderColor() {
        compare(input.background.border.color, input.borderColorNormal)
    }
}
