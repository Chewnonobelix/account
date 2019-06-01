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
    signal s_budgetRoleChange(string name, int role)
    signal s_addTarget(string cat)
    signal s_showTarget(string cat, string d, bool all)

    onWidthChanged: show()
    onHeightChanged: show()
    
    property bool blocked: false

    AccountStyle {
        id: pageStyle
    }
    
    function addCat(cat) {
        categoryModel.append(cat)
    }

    function addTarget(targ) {
        targetModel.append(targ)
    }
    
    function addSub(sub) {
        subModel.append(sub)
    }

    function clearCat() {
        categoryModel.clear()
    }
    
    function clearTarget() {
        targetModel.clear()
    }

    function clearSub() {
        subModel.clear()
    }

    function selectCat(name) {
        catView.currentIndex = categoryModel.getIndex(name)
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
        
        onCountChanged: catView.currentIndex = -1
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
            
            currentIndex: -1
            
            delegate: Rectangle {
                width: parent.width
                height: 40
                id: delItem

                gradient: categoryModel.getIndex(catName) === catView.currentIndex ? pageStyle.calSelect : pageStyle.unselectView
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
                            catView.currentIndex = index

                            if(categoryModel.get(catView.currentIndex).has)
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

                        onCurrentRoleChanged: {
                            if(!budgetManager.blocked)
                                budgetManager.s_budgetRoleChange(catName, currentRole)
                        }
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
            visible: catView.currentIndex !== -1 && categoryModel.get(catView.currentIndex).has


            model: targetModel
            currentIndex: -1
            
            
            onCurrentIndexChanged: {

                var temp = currentIndex !== - 1 ? Qt.formatDate(targetModel.get(currentIndex).date, "dd-MM-yyyy") : ""
                budgetManager.s_showTarget(categoryModel.get(catView.currentIndex).catName, temp, currentIndex === -1)
            }

            Component {
                id: compRemoveAction
                Control2.Action {
                    id: removeAction
                    text:  "Remove target"
                    
                    onTriggered: console.log(targetView.currentIndex, targetModel.get(targetView.currentIndex).date)
                }
            }

            Control2.Menu {
                id: targetItemMenu
                
                
                Control2.Action {
                    text: "Add target"
                    onTriggered: budgetManager.s_addTarget(categoryModel.get(catView.currentIndex).catName)
                }

            }

            delegate: Rectangle {
                width: parent.width
                height: 40

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
                    z:5
                    onClicked: {
                        targetView.currentIndex = targetView.currentIndex === index ? -1 :  index
                    }
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
                            targetView.currentIndex = targetView.indexAt(mouse.x, mouse.y)
                            if(targetView.currentIndex !== -1 && targetItemMenu.count === 1) {
                                targetItemMenu.addAction(compRemoveAction.createObject())
                            } else if (targetView.currentIndex === -1 && targetItemMenu.count === 2){
                                targetItemMenu.takeAction(1)
                            }

                            targetItemMenu.popup()
                        }
                    }
                }
            }
        }


        ListView {
            id: subView
            anchors.left:  targetView.right
            anchors.leftMargin: 10
            height: parent.height * .85
            width: parent.width * .30

            visible: targetView.visible
            model: subModel

            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: "gold"
            }

            delegate: Rectangle {
                height: 70
                width: subView.width
                color: "transparent"
                Column {
                    anchors.fill: parent
                    spacing: 10
                    Label {
                        width: parent.width
                        text: Qt.formatDate(begin, "dd-MM-yyyy") + " -> " + Qt.formatDate(end, "dd-MM-yyyy")
                    }

                    BudgetViewItem {
                        clip: true
                        width: parent.width
                        to: target
                        realValue: current
                        title: cat
                    }
                }
            }
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
