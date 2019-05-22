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
    signal s_loadTarget(string cat)
    signal s_budgetRoleChange(string nam, int role)

    onWidthChanged: show()
    onHeightChanged: show()
    
    AccountStyle {
        id: pageStyle
    }
    
    function addCat(cat) {
        categoryModel.append(cat)
    }

    function addTarget(targ) {
        targetModel.append(targ)
    }
    
    function clearCat() {
        categoryModel.clear()
    }
    
    function clearTarget() {
        targetModel.clear()
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
    }
    
    ListModel {
        id: subModel
    }
    
    Rectangle {
        anchors.fill: parent
        gradient: pageStyle.backgroundGradient

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
                height: 30
                Text {
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

                            if(categoryModel.get(catView.newCurrentIndex).has)
                                budgetManager.s_loadTarget(catName)
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


                    Control2.Menu {
                        id: freqMenu
                        enabled: has
                        property string val: ""
                        property int currentRole: -1

                        Component.onCompleted: {                            
                            if(frequency > 0) {
                                for(var i = 0; i < count; i++) {
                                    if(actionAt(i).role === frequency) {
                                        currentRole = frequency
                                        val = actionAt(i).text
                                    }
                                }
                            }
                        }

                        onCurrentRoleChanged: budgetManager.s_budgetRoleChange(catName, currentRole)
                        title: qsTr("Set to: ") + val

                        Control2.Action {
                            text: "Day"
                            property int role: 1
                            onTriggered: {
                                freqMenu.val = text
                                freqMenu.currentRole = role
                            }
                        }
                        Control2.Action {
                            text: "Week"
                            property int role: 2
                            onTriggered: {
                                freqMenu.val = text
                                freqMenu.currentRole = role
                            }
                        }
                        Control2.Action {
                            text: "Month"
                            property int role: 3
                            onTriggered: {
                                freqMenu.val = text
                                freqMenu.currentRole = role
                            }
                        }
                        Control2.Action {
                            property int role: 4
                            text: "Quarter"
                            onTriggered: {
                                freqMenu.val = text
                                freqMenu.currentRole = role
                            }
                        }
                        Control2.Action {
                            text: "Year"
                            property int role: 5
                            onTriggered: {
                                freqMenu.val = text
                                freqMenu.currentRole = role
                            }
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
        
        ListView {
            id: targetView
            objectName: "targetView"
            anchors.left: catView.right
            anchors.leftMargin: 10
            anchors.top: parent.top

            height: parent.height * .85
            width: parent.width * .30
            visible: catView.newCurrentIndex !== -1 && categoryModel.get(catView.newCurrentIndex).has


            model: targetModel
            currentIndex: -1
            
            delegate: Rectangle {
                width: parent.width
                height: 40
//                color: "transparent"
                gradient: targetView.currentIndex === index ? pageStyle.calSelect : pageStyle.unselectView
                Label {
                    id: targetText
                    anchors.fill: parent
                    text: Qt.formatDate(date, "dd-MM-yyyy") + ", " + target
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    fontSizeMode: Text.Fit
                    font.family: pageStyle.core.name
                    font.pixelSize: pageStyle.core.size                    
                }
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    z:1 
                    onClicked: {
                        targetView.currentIndex = index
                        mouse.accepted = false
                    }
                    propagateComposedEvents: true
                }
            }

            Rectangle {
                anchors.fill: parent
                border.color: "blue"
                color: "transparent"
                
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
                    z: -1
                    onClicked: {
                        if(mouse.button === Qt.RightButton) {
                            console.log("Right tartget")
                        }
                    }
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
