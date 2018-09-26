import QtQuick 2.0
import QtQuick.Controls 2.4

SpinBox {
    id: spinbox
    from: 0
    value: 110
    to: 1000000000
    stepSize: 10
    editable: true

    property int decimals: 2
    property real realValue: value / 100

    validator: DoubleValidator {
        bottom: Math.min(spinbox.from, spinbox.to)
        top:  Math.max(spinbox.from, spinbox.to)
    }

    textFromValue: function(value, locale) {
        return Number(value / 100).toLocaleString(locale, 'f', spinbox.decimals)
    }

    valueFromText: function(text, locale) {
        return Number.fromLocaleString(locale, text) * 100
    }

    contentItem: TextInput {
            z: 2
            text: spinbox.textFromValue(spinbox.value, spinbox.locale)

            font: spinbox.font
//                    color: "transparent"
            selectionColor: "#21be2b"
            selectedTextColor: "#ffffff"
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter

            readOnly: !spinbox.editable
            validator: spinbox.validator
            inputMethodHints: Qt.ImhFormattedNumbersOnly

        }

        up.indicator: Rectangle {
            x: spinbox.mirrored ? 0 : parent.width - width
            height: parent.height
            implicitWidth: 40
            implicitHeight: 40
            border.color: enabled ? "#21be2b" : "#bdbebf"
            gradient: enabled ? pageStyle.goldButton: pageStyle.unselectView
            Text {
                text: ">"
                font.pixelSize: spinbox.font.pixelSize * 2
                color: enabled ? "black" : "grey"
                anchors.fill: parent
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        down.indicator: Rectangle {
            x: spinbox.mirrored ? parent.width - width : 0
            height: parent.height
            implicitWidth: 40
            implicitHeight: 40
            border.color: enabled ? "#21be2b" : "#bdbebf"
            gradient: enabled ? pageStyle.goldButton: pageStyle.unselectView

            Text {
                text: "<"
                font.pixelSize: spinbox.font.pixelSize * 2
                color: enabled ? "black" : "grey"
                anchors.fill: parent
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        background: Rectangle {
            color: "transparent"
            implicitWidth: 140
            border.color: "#bdbebf"
        }
}

