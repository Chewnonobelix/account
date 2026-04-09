import QtQuick
import QtQuick.Controls

import "../Style" as Style

Button {
    id: root

    implicitWidth: 140
    implicitHeight: 40

    text: "placeholder button"
    background: Rectangle {
        radius: 6
        border.width: 1
        border.color: !root.enabled ? Style.OBTheme.palette.disabled :
                                      root.pressed ? Style.OBTheme.palette.outlineStrong : Style.OBTheme.palette.outline
        gradient: !root.enabled ? Style.OBStyle.silver : root.pressed ? Style.OBStyle.goldIn : Style.OBStyle.goldOut
    }

    contentItem: OBLabel {
        text: root.text
        textState: root.enabled ? OBLabel.TextState.Neutral : OBLabel.TextState.Disabled
    }

    onClicked: console.log("Button %1 test click".arg(text))
}
