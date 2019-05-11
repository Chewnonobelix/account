import QtQuick 2.12
import QtQuick.Controls 2.5 as Control2
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
import QtQuick.controls.Styles 1.4
import "../Style"

Window {
    height: 640
    width: 480
    flags: Qt.Window
    x: screen.width / 2 - width / 2
    y: screen.height / 2 - height / 2
    
    id: budgetManager

    signal s_budgetChanged(string name)
    signal s_budgetReference(string name)

    onWidthChanged: show()
    onHeightChanged: show()
    
    AccountStyle {
        id: pageStyle
    }
    
    function add(cat) {
        categoryModel.append(cat)
    }
    
    function clear() {
        categoryModel.clear()
    }
    
    ListModel {
        id: categoryModel

        function getIndex(name) {
            var ret = -1;

            for(var i = 0; i < count; i++) {
                if(get(i).catName === name)
                    ret = i;
            }

            return ret
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
        
        ListView {
            id: catView
            model: categoryModel
            anchors.left: parent.left
            anchors.top: parent.top
            
            height: parent.height * .85
            width: parent.width * .30
            
//            backgroundVisible: false
            

            section.property: "type"
            section.criteria: ViewSection.FullString
            section.delegate: Rectangle {
                gradient: pageStyle.goldHeader
                width: parent.width
                height: parent.height * .15
                Label {
                    anchors.fill: parent
                    text: section
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    fontSizeMode: Text.Fit
                    font.family: pageStyle.title.name
                    font.pixelSize: pageStyle.title.size2
                }
            }
            
            section.labelPositioning: ViewSection.InlineLabels
            
            property int newCurrentIndex: -1

            delegate: Rectangle {
                width: parent.width
                height: 40
                id: delItem

                gradient: categoryModel.getIndex(catName) === catView.newCurrentIndex ? pageStyle.calSelect : pageStyle.unselectView
                Label {
                    id: catDisplay
                    color: has ? "black" : "red"
                    anchors.centerIn: parent
                    text: catName
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    fontSizeMode: Text.Fit
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onClicked: {
                        if(mouse.button === Qt.LeftButton) {
                            catView.newCurrentIndex = categoryModel.getIndex(catName)
                                      }
                        else {
                            catMenu.popup()
                        }
                    }
                }

                Control2.Menu {
                    id: catMenu

                    Control2.MenuItem {
                        text: has ? "Remove budget" : "Add budget"

                        onTriggered: budgetManager.s_budgetChanged(catName)
                        background: Rectangle {
                            gradient: parent.highlighted ? pageStyle.darkGoldButton : pageStyle.goldButton
                        }
                    }

                    Control2.MenuItem {
                        text: "Edit budget reference"
                        enabled: has
                        onTriggered: budgetManager.s_budgetReference(catName)
                        background: Rectangle {
                            gradient: parent.highlighted ? pageStyle.darkGoldButton : pageStyle.goldButton

                        }
                    }
                }
            }
                        

            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: "gold"
            }    
        }
        
        
        //        Button {
        //            id: addCat
        //            width: catView.width / 2 - 5
        //            height: parent.height * 0.10
        
        //            anchors.top: catView.bottom
        //            anchors.topMargin: parent.height * 0.05
        //            anchors.left: catView.left
        
        //            text: "+"
        //            style: ButtonStyle {
        //                background: Rectangle {
        //                    gradient: addCat.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
        //                }
        //            }
        //        }
        
        //        Button {
        //            id: removeCat
        //            x: 77
        //            y: 576
        //            width: catView.width / 2 - 5
        //            height: parent.height * 0.10
        
        //            anchors.top: catView.bottom
        //            anchors.topMargin: parent.height * 0.05
        //            anchors.left: addCat.right
        //            anchors.leftMargin: 10
        //            text: "-"
        
        //            style: ButtonStyle {
        //                background: Rectangle {
        //                    gradient: addCat.pressed ? pageStyle.darkGoldButton : pageStyle.goldButton
        //                }
        //            }
        //        }
        
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
