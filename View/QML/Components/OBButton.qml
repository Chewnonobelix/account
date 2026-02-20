import QtQuick
import QtQuick.Controls

import "../Style" as OBStyle

Button {
    id: root

    background: Rectangle {
        gradient: !root.enabled ? OBStyle.OBStyle.silver : root.pressed ? OBStyle.OBStyle.goldIn : OBStyle.OBStyle.goldOut
    }

    contentItem: OBLabel {
        text: root.text
        textState: OBLabel.TextState.Neutral
    }
}
