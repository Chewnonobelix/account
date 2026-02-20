import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import "../Style" as OBStyle
Control {
    id: root

    enum TextState {
        Neutral,
        Positive,
        Negative
    }

    property alias text: textID.text
    property TextState textState: TextState.Neutral

    Label {
        id: textID
        anchors.fill: parent
        text: "placeholder"
        verticalAlignment: Qt.AlignVCenter
    }

    Rectangle {
        anchors.fill: parent
        gradient: root.textState === TextState.Negative ? OBStyle.OBStyle.negative :
                                                          root.textState === TextState.Positive ? OBStyle.OBStyle.positive : OBStyle.OBStyle.black
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: textID
        }
    }
}
