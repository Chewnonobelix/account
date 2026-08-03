import QtQuick
import QtTest

import "../../QML/ComposedComponent" as Composed
import "../../QML/Components" as Comp

TestCase {
    id: testCase
    name: "DescLine"
    when: windowShown
    width: 300
    height: 100

    Composed.DescLine {
        id: descLine
        title: "Name"

        Comp.OBTextInput {
            id: valueInput
            text: "Alice"
        }
    }

    Composed.DescLine {
        id: presetLine
        title: "Balance"
        contentType: Composed.DescLine.ContentType.SpinBox
        contentConfig: ({ "realTo": 1000, "realValue": 250 })
    }

    function test_titleIsSet() {
        compare(descLine.title, "Name")
    }

    function test_contentHoldsTheDeclaredChild() {
        compare(descLine.content.length, 1)
        compare(descLine.content[0], valueInput)
    }

    function test_presetInstantiatesConfiguredComponent() {
        compare(presetLine.value.realTo, 1000)
        compare(presetLine.value.realValue, 250)
    }
}
