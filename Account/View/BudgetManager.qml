import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
import QtQuick.controls.Styles 1.4

Window {
    Component.onCompleted: console.log("bb")
    height: 640
    width: 480
    flags: Qt.Window
    x: 640
    y: 50
    //        y: 480
    AccountStyle {
        id: pageStyle
    }

    ListModel {
        id: categoryModel
        ListElement {
            catName: "Cat 1"
        }
        ListElement {
            catName: "Cat 2"
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

    Rectangle {
        anchors.fill: parent
        gradient: pageStyle.backgroundGradient

        //        TableView {
        //            id: subView
        //            enabled: subModel.count != 0
        //            anchors.left: targetView.right
        //            anchors.leftMargin: 10
        //            anchors.top: parent.top

        //            height: parent.height * .85
        //            width: parent.width * .35
        //            //            flickingHorizontally: false
        //            Rectangle {
        //                anchors.fill: parent
        //                color: "transparent"
        //                border.color: "gold"
        //            }
        //        }

        TableView {
            id: catView
            model: categoryModel
            anchors.left: parent.left
            anchors.top: parent.top

            height: parent.height * .85
            width: parent.width * .30

            backgroundVisible: false
            TableViewColumn {
                title: "Category name"
                role: "catName"

                delegate: Rectangle {
                    width: parent.width
                    color: "transparent"
                    Label {
                        id: catDisplay

                        anchors.fill: parent
                        text: styleData.value
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            catView.selection.clear()
                            catView.selection.select(styleData.row)
                            console.log(styleData.row, catView.selection.count)
                        }
//                        acceptedButtons: Qt.NoButton
//                        onHoveredChanged: console.log(catDisplay.text)
                    }
                }


            }

            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: "gold"
            }

            style: TableViewStyle {
                rowDelegate: Rectangle {
                    color: "transparent"
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
            style: ButtonStyle {
                background: Rectangle {
                    gradient: addCat.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }
            }
        }

        Button {
            id: removeCat
            x: 77
            y: 576
            width: catView.width / 2 - 5
            height: parent.height * 0.10

            anchors.top: catView.bottom
            anchors.topMargin: parent.height * 0.05
            anchors.left: addCat.right
            anchors.leftMargin: 10
            text: "-"

            style: ButtonStyle {
                background: Rectangle {
                    gradient: addCat.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }
            }
        }

        //        TableView {
        //            id: targetView
        //            enabled: categoryModel.count != 0
        //            anchors.left: catView.right
        //            anchors.leftMargin: 10
        //            anchors.top: parent.top

        //            height: parent.height * .85
        //            width: parent.width * .30

        //            model: targetModel

        //            Component.onCompleted: console.log(model.count)
        //            Rectangle {
        //                anchors.fill: parent
        //                color: "transparent"
        //                border.color: "gold"
        //            }

        //            delegate: Rectangle {
        //                clip: true
        //                //                width: parent.width
        //                //                height: parent.height*0.10
        //                //                anchors.top: parent.top
        //                //                anchors.left: catView.left
        //                //                anchors.fill: parent
        //                Label {
        //                    anchors.fill: parent
        //                    text: targetDate + ": " + targetValue
        //                    horizontalAlignment: Text.AlignHCenter
        //                    verticalAlignment: Text.AlignVCenter
        //                }
        //            }
        //        }

        //        Button {
        //            id: addTarget
        //            width: targetView.width / 2 - 5
        //            height: parent.height * 0.10

        //            anchors.top: targetView.bottom
        //            anchors.topMargin: parent.height * 0.05
        //            anchors.left: targetView.left

        //            text: "+"

        //            background: Rectangle {
        //                gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
        //            }
        //        }

        //        Button {
        //            id: removeTarget
        //            width: targetView.width / 2 - 5
        //            height: parent.height * 0.10

        //            anchors.top: targetView.bottom
        //            anchors.topMargin: parent.height * 0.05
        //            anchors.left: addTarget.right
        //            anchors.leftMargin: 10
        //            text: "-"

        //            background: Rectangle {
        //                gradient: parent.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
        //            }
        //        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
