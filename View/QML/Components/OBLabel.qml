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
    // Every consumer that constrains this to a fixed/preferred width
    // (combo boxes, table cells, DescLine values, ...) risks text wider
    // than that box; elide it instead of letting it paint over neighbors.
    elide: Text.ElideRight

    state: textState === OBLabel.TextState.Negative ? "negative" :
           textState === OBLabel.TextState.Positive ? "positive" :
           textState === OBLabel.TextState.Disabled ? "disabled" : "neutral"

    states: [
        State {
            name: "neutral"
            PropertyChanges { target: root; color: root.colorNeutral }
        },
        State {
            name: "positive"
            PropertyChanges { target: root; color: root.colorPositive }
        },
        State {
            name: "negative"
            PropertyChanges { target: root; color: root.colorNegative }
        },
        State {
            name: "disabled"
            PropertyChanges { target: root; color: root.colorDisabled }
        }
    ]
}
