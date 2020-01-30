import QtQuick 2.14

Rectangle {
    gradient: invisible ? AccountStyle.unselectView : AccountStyle.backgroundGradient
    border.color: "gold"
    property bool invisible: false
}
