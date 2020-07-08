import QtQuick 2.12
import QtQuick.Controls 2.5 as Control2
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
import QtQuick.controls.Styles 1.4
import "../Style"

Rectangle {
    id: root
    
    signal s_budgetChanged(string name)
    signal s_budgetReference(string name)
    signal s_loadTarget(string cat)
    signal s_budgetRoleChange(string name, int role)
    signal s_addTarget(string cat)
    signal s_showTarget(string cat, string d, bool all)
    signal s_removeTarget(string cat, string d)
    
    property bool blocked: false
    
    color: "transparent"
        
    ReferenceView {
        objectName: "reference"
        anchors.centerIn: parent
        height: width * 1.33
        width: 150 * 1.5
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
        targetView.currentIndex = -1
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
    
    RowLayout {
        anchors.fill: parent
        
        spacing: width * 0.02
        
        ListView {
            id: catView
            model: categoryModel
            
            Layout.preferredHeight:  root.height * .85
            Layout.preferredWidth: root.width * .30
            Layout.alignment: Qt.AlignTop
            
            section.property: "type"
            section.criteria: ViewSection.FullString
            section.delegate: Rectangle {
                gradient: AccountStyle.goldHeader
                width: parent.width
                height: 30
                Text {
                    anchors.fill: parent
                    text: section
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    fontSizeMode: Text.Fit
                    font.family: AccountStyle.title.name
                    font.pixelSize: AccountStyle.title.size2
                }
            }
            
            section.labelPositioning: ViewSection.InlineLabels
            
            
            currentIndex: -1
            clip: true
            
            delegate: Rectangle {
                width: parent.width
                height: 40
                id: delItem
                
                gradient: categoryModel.getIndex(catName) === catView.currentIndex ? AccountStyle.calSelect : AccountStyle.unselectView
                AccountLabel {
                    id: catDisplay
                    color: has ? "black" : "red"
                    anchors.centerIn: parent
                    text: catName
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
                                root.s_loadTarget(catName)
                        }
                        else {
                            catMenu.popup()
                        }
                    }
                }
                
                Control2.Menu {
                    id: catMenu
                    
                    height: delItem.height
                    
                    delegate: Control2.MenuItem {
                        font.family: AccountStyle.core.name
                        font.pixelSize: AccountStyle.core.size
                                  
                        height: catMenu.height
                        background: Rectangle {
                            anchors.fill: parent
                            gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
                        }
                    }
                    
                    Control2.Action {
                        id: addBudget
                        text:  has ? qsTr("Remove budget") : qsTr("Add budget")
                        onTriggered: root.s_budgetChanged(catName)
                    }
                    
                }
            }
            
            
            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: "darkseagreen"
            }
        }
        
        ListView {
            id: targetView
            objectName: "targetView"
            
            Layout.preferredHeight: root.height * .85
            Layout.preferredWidth: root.width * .30
            Layout.alignment: Qt.AlignTop
            visible: catView.currentIndex !== -1 && categoryModel.get(catView.currentIndex).has
            
            
            model: targetModel
            currentIndex: -1
            clip: true            
            spacing: height * 0.02
            
            header: AccountHeader {
                text: qsTr("Target")
                width: targetView.width
                height: targetView.height * .07
            }

            onCurrentIndexChanged: {
                var temp = currentIndex !== - 1 ? Qt.formatDate(targetModel.get(currentIndex).date, "dd-MM-yyyy") : ""
                root.s_showTarget(categoryModel.get(catView.currentIndex).catName, temp, currentIndex === -1)
            }
            
            Component {
                id: compRemoveAction
                Control2.Action {
                    id: removeAction
                    text:  qsTr("Remove target")
                    
                    onTriggered: {
                        s_removeTarget(categoryModel.get(catView.currentIndex).catName, Qt.formatDate(targetModel.get(targetView.currentIndex).date, "dd-MM-yyyy"))
                    }
                }
            }
            
            Control2.Menu {
                id: targetItemMenu
                
                height: count * 20
                
                delegate: Control2.MenuItem {
                    font.family: AccountStyle.core.name
                    font.pixelSize: AccountStyle.core.size
                    height: 20
                    background: Rectangle {
                        gradient: parent.highlighted ? AccountStyle.darkGoldButton : AccountStyle.goldButton
                    }
                }
                
                Control2.Action {
                    id: addSubTarget
                    text: qsTr("Add target")
                    onTriggered: root.s_addTarget(categoryModel.get(catView.currentIndex).catName)
                }
            }
            
            delegate: Rectangle {
                width: parent.width
                height: 40
                
                border.color: "gray"
                gradient: targetView.currentIndex === index ? AccountStyle.calSelect : AccountStyle.backgroundGradient
                AccountLabel {
                    id: targetText
                    anchors.fill: parent
                    text: qsTr("Date") + ": " + Qt.formatDate(date, "dd-MM-yyyy") + "\n" + qsTr("Target") + ": " + target + Qt.locale().currencySymbol(Locale.CurrencySymbol)
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
                border.color: "darkseagreen"
                color: "transparent"
                
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
                    z: -1
                    onClicked: {
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
         
        ListView {
            id: subView

            Layout.preferredHeight: root.height * .85
            Layout.preferredWidth: root.width * .30
            Layout.alignment: Qt.AlignTop
            
            visible: targetView.visible
            model: subModel
            clip: true
            spacing: height * 0.02
            
            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: "darkseagreen"
            }
            
            header: AccountHeader {
                text: qsTr("All sub budget")
                width: subView.width
                height: subView.height * .07
            }

            
            delegate: Rectangle {
                height: 60
                width: subView.width
                gradient: AccountStyle.backgroundGradient
                border.color: "gray"
                
                Column {
                    anchors.fill: parent
                    AccountLabel {
                        width: parent.width
                        height: 25
                        text: Qt.formatDate(begin, "dd-MM-yyyy") + " -> " + Qt.formatDate(end, "dd-MM-yyyy")
                    }
                    
                    BudgetViewItem {
                        anchors.bottomMargin: 5
                        clip: true
                        height: 30
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
