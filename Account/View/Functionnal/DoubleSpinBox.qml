import QtQuick 2.13
import QtQuick.Controls 2.4

import "../Style"

SpinBox {
    id: spinbox
    from: 0
    value: 0
    to: 100000000
    stepSize: 10
    editable: true

    property int decimals: 2
    property real realValue: value / 100
    property bool isEditing: input.cursorVisible
    
    signal s_realVCalueChange(real value)
    
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
    
    function increase() {
        value = value+stepSize
        s_realVCalueChange(realValue)
    }

    function decrease() {
        value = value-stepSize
        s_realVCalueChange(realValue)
    }
    
    contentItem: TextInput {
            id: input
            z: 2
            text: spinbox.textFromValue(spinbox.value, spinbox.locale)

            font: spinbox.font

            selectionColor: "#21be2b"
            selectedTextColor: "#ffffff"
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter

            readOnly: !spinbox.editable
            validator: spinbox.validator
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            onEditingFinished: {
                spinbox.value = spinbox.valueFromText(text, spinbox.locale)
                cursorVisible = false
                s_realVCalueChange(realValue)
            }
        }

        up.indicator: Rectangle {
            x: spinbox.mirrored ? 0 : parent.width - width
            height: parent.height
            width: parent.width * 0.25
            border.color: enabled ? "#21be2b" : "#bdbebf"
            gradient: enabled ? up.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton: AccountStyle.unselectView


            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                cursorShape: parent.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
                
                onPressed: increase()
                
            }

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
            width: parent.width * 0.25
            border.color: enabled ? "#21be2b" : "#bdbebf"
            gradient: enabled ? down.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton: AccountStyle.unselectView

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                cursorShape: parent.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
                
                onPressed: decrease()
            }

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

