import QtQuick
import QtTest

import "../../QML/Components" as Comp

TestCase {
    id: testCase
    name: "OBTitle"
    when: windowShown
    width: 200
    height: 100

    Comp.OBTitle {
        id: title
    }

    function test_defaultText() {
        compare(title.text, "Title")
        compare(title.displayText, "TITLE")
    }

    function test_textIsAlwaysUppercase() {
        title.text = "hello world"
        compare(title.text, "hello world")
        compare(title.displayText, "HELLO WORLD")
    }

    function test_mixedCaseTextIsUppercased() {
        title.text = "MiXeD CaSe"
        compare(title.displayText, "MIXED CASE")
    }
}
