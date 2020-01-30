import QtQuick 2.11
import QtQuick.Controls 2.4
import "../Style"
Popup {
    implicitHeight: parent.height
    implicitWidth: parent.width * .33

    anchors.centerIn: parent
    id: checker

    signal validate()
    
    Label {
        text: qsTr("Entry to check")
        font.pixelSize: AccountStyle.title.size
        font.family: AccountStyle.title.name
        fontSizeMode: Text.Fit
        id: header
        anchors.left: parent.left
        anchors.right: parent.right
    }

    background: Rectangle {
        gradient: AccountStyle.backgroundGradient
        border.color: "gold"
    }

    property var tab: []
    
    ScrollView {
        id: listChecker
        width: parent.width
        height: parent.height * .90
        anchors.top: header.bottom
        anchors.topMargin: 10
        //        flickableDirection: Flickable.AutoFlickDirection
        //        vertical.policy: Qt.ScrollBarAsNeeded

        clip: true
        Column {
            clip: true
            Repeater {
                objectName: "repeater"
                model: []
                clip: true
                delegate: Row {
                    id: row
                    clip: true
                    
                    Component.onCompleted: tab[modelData.id] = false
                    
                    CheckBox {
                        id: rowChecked
                        checked: false
                        font.family: AccountStyle.core.name
                        font.pointSize: AccountStyle.core.size

                        MouseArea {
                            z: -1
                            acceptedButtons: Qt.NoButton
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                        }

                        onCheckedChanged: {
                            tab[modelData.id] = checked;
                        }
                    }


                    Label {
                        font.family: AccountStyle.core.name
                        font.pointSize: AccountStyle.core.size
                        anchors.verticalCenter: rowChecked.verticalCenter
                        text: modelData.label + " " + Qt.formatDate(modelData.date, "dd-MM-yyyy") + " " + modelData.value + "€"

                        MouseArea {
                            acceptedButtons: Qt.LeftButton
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true
                            onClicked: rowChecked.checked = !rowChecked.checked
                            propagateComposedEvents: true

                            ToolTip.text: qsTr("Select transaction for validation")
                            ToolTip.delay: 500
                            ToolTip.visible: containsMouse
                        }
                    }
                }
            }
        }
    }

    AccountButton {
        id: save
        text: qsTr("Save")
        anchors.bottom: parent.bottom
        height: parent.height * .05

        onClicked:  {
            checker.validate()
        }
    }

    AccountButton {
        text: qsTr("Later")
        anchors.left: save.right
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        height: parent.height * .05

        ToolTip.text: qsTr("Keep validation for later")
        ToolTip.visible: hovered
        ToolTip.delay: 500

        onClicked: {
            close()
        }
    }
}
