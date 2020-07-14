import QtQuick 2.14

Rectangle {
    gradient: invisible ? AccountStyle.unselectView : AccountStyle.backgroundGradient
    border.color: borderEnabled ? "gold" : "transparent"
    property bool invisible: false
    property bool borderEnabled: true
}
