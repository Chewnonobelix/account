import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

Popup {
    Component.onCompleted: console.log("aaaa")
    //    height: 640
    //    width: 480
    AccountStyle {
        id: pageStyle
    }
    background: Rectangle {
        gradient: pageStyle.backgroundGradient
    }

    ListModel {
        id: categoryModel
    }

    ListModel {
        id: targetModel
    }

    ListModel {
        id: subModel
    }

    ColumnLayout {
        id: cvl
        ListView {
            id: categoryView
            model: categoryModel
            delegate: Rectangle {
                Label {
                    text: catName
                }
            }
        }
        RowLayout {
            Button {
                id: addCat
                text: "+"
            }

            Button {
                id: removeCat
                text: "-"
            }
        }
    }

    ColumnLayout {
        anchors.left: cvl.right
        anchors.leftMargin: 20
        ListView {
            id: targetView
            enabled: targetModel.count != 0 && categoryModel.count != 0
        }
        RowLayout {
            Button {
                id: addTarget
                text: "+"
                background: Rectangle {
                    gradient: addTarget.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
                }
            }

            Button {
                id: removeTarget
                text: "-"
            }
        }
    }

    ListView {
        id: subView
        enabled: subModel.count != 0
    }
}
