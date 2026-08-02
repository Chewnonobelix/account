import QtQuick

import "../Style" as Style

Text {
    id: root

    enum TextState {
        Neutral,
        Positive,
        Negative,
        Disabled
    }

    property int textState: OBLabel.TextState.Neutral

    property color colorNeutral: Style.OBTheme.palette.textPrimary
    property color colorPositive: Style.OBTheme.palette.success
    property color colorNegative: Style.OBTheme.palette.danger
    property color colorDisabled: Style.OBTheme.palette.textDisabled

    text: "Label"
    verticalAlignment: Text.AlignVCenter
    color: textState === OBLabel.TextState.Negative ? colorNegative :
           textState === OBLabel.TextState.Positive ? colorPositive :
           textState === OBLabel.TextState.Disabled ? colorDisabled : colorNeutral
}
