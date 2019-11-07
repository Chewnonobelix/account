import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.12
import "../Style" as S
import "../Frequency" as F

Item {
    id: info
    objectName: "info"
    property int maximum: Screen.width * .55 - 10
    width: maximum
    
    
    S.AccountStyle {
        id: pageStyle
    }
    
    ColumnLayout {
        anchors.fill: parent
        EntryEdit {
            id: ee
            objectName: "entryEdit"
            visible: parent.visible
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height * .15
        }
        spacing: 10
        Label {
            text: qsTr("Coming Soon")
            Layout.fillHeight: true
            Layout.fillWidth: true
            MouseArea {
                anchors.fill: parent
                onClicked:  {
                    console.log(screen.desktopAvailableHeight, screen.desktopAvailableWidth)                    
                    ee.changeDirection()
                }
            }
        }
    }
}
