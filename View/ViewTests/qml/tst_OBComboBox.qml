import QtQuick
import QtTest

import "../../QML/Components" as Comp
import "../../QML/Style" as Style

TestCase {
    id: testCase
    name: "OBComboBox"
    when: windowShown
    width: 300
    height: 200

    Comp.OBComboBox {
        id: combo
        model: ["Checking", "Savings", "Credit card"]
    }

    function test_defaultSize() {
        compare(combo.implicitWidth, Style.OBConstants.widthMedium)
        compare(combo.implicitHeight, Style.OBConstants.heightMedium)
    }

    function test_modelCount() {
        compare(combo.count, 3)
    }

    function test_currentIndexUpdatesDisplayText() {
        combo.currentIndex = 1
        compare(combo.displayText, "Savings")
    }

    function test_contentItemShowsDisplayText() {
        combo.currentIndex = 2
        compare(combo.contentItem.text, "Credit card")
    }
}
