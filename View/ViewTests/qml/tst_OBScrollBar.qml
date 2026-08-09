import QtQuick
import QtQuick.Controls
import QtTest

import "../../QML/Components" as Comp
import "../../QML/Style" as Style

TestCase {
    id: testCase
    name: "OBScrollBar"
    when: windowShown
    width: 300
    height: 200

    Comp.OBScrollBar {
        id: bar
    }

    function init() {
        bar.orientation = Qt.Vertical
        bar.policy = ScrollBar.AsNeeded
        bar.size = 0.5
        bar.position = 0
    }

    function test_defaultOrientationIsVerticalSized() {
        compare(bar.orientation, Qt.Vertical)
        compare(bar.implicitWidth, bar.thickness)
        compare(bar.implicitHeight, Style.OBConstants.widthBig)
    }

    function test_horizontalOrientationSwapsImplicitSize() {
        bar.orientation = Qt.Horizontal
        compare(bar.implicitWidth, Style.OBConstants.widthBig)
        compare(bar.implicitHeight, bar.thickness)
    }

    function test_defaultGradients() {
        compare(bar.handleGradientNormal, Style.OBStyle.goldOut)
        compare(bar.handleGradientActive, Style.OBStyle.goldIn)
        compare(bar.handleGradientDisabled, Style.OBStyle.silver)
        compare(bar.grooveGradient, Style.OBStyle.silver)
    }

    function test_alwaysOnPolicyIsShown() {
        bar.policy = ScrollBar.AlwaysOn
        compare(bar.shown, true)
    }

    function test_asNeededPolicyHiddenWhenInactive() {
        bar.policy = ScrollBar.AsNeeded
        compare(bar.active, false)
        compare(bar.shown, false)
    }

    function test_fullSizeNeverShownEvenIfActive() {
        bar.policy = ScrollBar.AsNeeded
        bar.size = 1.0
        compare(bar.shown, false)
    }
}
