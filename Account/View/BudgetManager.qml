import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Popup {
    Component.onCompleted: console.log("bb")
        height: 640
        width: 480

        x: 640
//        y: 480
    AccountStyle {
        id: pageStyle
    }
    background: Rectangle {
        gradient: pageStyle.backgroundGradient
    }

    Rectangle {
        color: "transparent"
        anchors.fill: parent

        ListModel {
            id: categoryModel
            ListElement {
                catName: "Cat 1"
            }

        }

        ListModel {
            id: targetModel
            ListElement {
                targetDate: "try1"
                targetValue: 10.0
            }

        }

        ListModel {
            id: subModel
        }

        ListView {
            id: subView
            enabled: subModel.count != 0
            anchors.left: targetView.right
            anchors.leftMargin: 10
            anchors.top: parent.top

            height: parent.height * .85
            width: parent.width * .35

            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: "gold"
            }

        }

        ListView {
            id: catView
            model: categoryModel
//            enabled: subModel.count != 0
            anchors.left: parent.left
            anchors.top: parent.top

            height: parent.height * .85
            width: parent.width * .30

            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: "gold"
            }

            delegate: Rectangle {
//                clip: true
//                width: parent.width
//                height: parent.height*0.10
//                anchors.top: parent.top
                anchors.fill: parent
//                anchors.left: catView.left

                Label {

//                    clip: true
                    anchors.fill: parent
                    text: catName
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    acceptedButtons: Qt.NoButton

                }

            }

        }

        Button {
            id: addCat
            width: catView.width / 2 - 5
            height: parent.height * 0.10

            anchors.top: catView.bottom
            anchors.topMargin: parent.height * 0.05
            anchors.left: catView.left

            text: "+"

            background: Rectangle {
                gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
            }
        }

        Button {
            id: removeCat
            width: catView.width / 2 - 5
            height: parent.height * 0.10

            anchors.top: catView.bottom
            anchors.topMargin: parent.height * 0.05
            anchors.left: addCat.right
            anchors.leftMargin: 10
            text: "-"

            background: Rectangle {
                gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
            }
        }


        ListView {
            id: targetView
            enabled: categoryModel.count != 0
            anchors.left: catView.right
            anchors.leftMargin: 10
            anchors.top: parent.top

            height: parent.height * .85
            width: parent.width * .30

            model: targetModel

            Component.onCompleted: console.log(model.count)
            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: "gold"
            }

            delegate: Rectangle {
                clip: true
//                width: parent.width
//                height: parent.height*0.10
//                anchors.top: parent.top
//                anchors.left: catView.left
                anchors.fill: parent
                Label {
                    anchors.fill: parent
                    text: targetDate + ": " + targetValue
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

        }

        Button {
            id: addTarget
            width: targetView.width / 2 - 5
            height: parent.height * 0.10

            anchors.top: targetView.bottom
            anchors.topMargin: parent.height * 0.05
            anchors.left: targetView.left

            text: "+"

            background: Rectangle {
                gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
            }
        }

        Button {
            id: removeTarget
            width: targetView.width / 2 - 5
            height: parent.height * 0.10

            anchors.top: targetView.bottom
            anchors.topMargin: parent.height * 0.05
            anchors.left: addTarget.right
            anchors.leftMargin: 10
            text: "-"

            background: Rectangle {
                gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
            }
        }


    }
}
