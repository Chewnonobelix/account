import QtQuick 2.0
import QtQuick.Controls 2.14

Label{
    color: "black"
    font.family: AccountStyle.title.name
    font.pixelSize: AccountStyle.title.size2
    fontSizeMode: Text.Fit
    clip: true
    horizontalAlignment: Qt.AlignHCenter
    verticalAlignment: Qt.AlignVCenter
    background: Rectangle {
        gradient: AccountStyle.goldHeader
    }
}
