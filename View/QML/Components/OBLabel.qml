pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import "../Style" as Style

Control {
    id: root

    enum TextState {
        Neutral,
        Positive,
        Negative,
        Disabled
    }

    implicitWidth: textID.implicitWidth
    implicitHeight: textID.implicitHeight

    property alias text: textID.text
    property int textState: OBLabel.TextState.Neutral

    Label {
        id: textID
        anchors.fill: parent
        text: "placeholder label"
        verticalAlignment: Qt.AlignVCenter
    }

    Rectangle {
        anchors.fill: parent
        gradient: root.textState === OBLabel.TextState.Negative ? Style.OBStyle.negative :
                                                                  root.textState === OBLabel.TextState.Positive ? Style.OBStyle.positive :
                                                                                                                   root.textState === OBLabel.TextState.Disabled ? Style.OBStyle.silver : Style.OBStyle.black
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: textID
        }
    }
}
