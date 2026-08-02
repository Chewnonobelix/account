import QtQuick
import QtTest

import "../../QML/Components" as Comp

TestCase {
    id: testCase
    name: "OBLabel"
    when: windowShown
    width: 200
    height: 100

    Comp.OBLabel {
        id: label
    }

    function test_defaultText() {
        compare(label.text, "Label")
    }

    function test_neutralUsesColorNeutral() {
        label.textState = Comp.OBLabel.TextState.Neutral
        compare(label.color, label.colorNeutral)
    }

    function test_positiveUsesColorPositive() {
        label.textState = Comp.OBLabel.TextState.Positive
        compare(label.color, label.colorPositive)
    }

    function test_negativeUsesColorNegative() {
        label.textState = Comp.OBLabel.TextState.Negative
        compare(label.color, label.colorNegative)
    }

    function test_disabledUsesColorDisabled() {
        label.textState = Comp.OBLabel.TextState.Disabled
        compare(label.color, label.colorDisabled)
    }

    function test_textUpdates() {
        label.text = "Hello"
        compare(label.text, "Hello")
    }
}
