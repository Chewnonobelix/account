import QtQuick
import QtTest

import "../../QML/Components" as Comp
import "../../QML/Style" as Style

TestCase {
    id: testCase
    name: "OBHeader"
    when: windowShown
    width: 300
    height: 200

    Comp.OBHeader {
        id: header
    }

    function test_defaultSize() {
        compare(header.implicitWidth, Style.OBConstants.widthBig)
        compare(header.implicitHeight, Style.OBConstants.heightBig)
    }

    function test_textAlias() {
        header.text = "Accounts"
        compare(header.text, "Accounts")
    }

    function test_defaultGradientIsHeaderGradient() {
        compare(header.gradient, Style.OBStyle.header)
    }
}
