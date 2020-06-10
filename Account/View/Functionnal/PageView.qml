import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

StackView {
    id: root
    
    //    property int index: 0
    
    
    PageIndicator {
        anchors.right: root.right
        anchors.verticalCenter: parent.verticalCenter
        id: pageChange
        count: root.children.length - 1
        currentIndex: -1
        z: root.children.length+10
        interactive: true
        
        background: Rectangle {
            border.color: "gold"
        }
        
        rotation: 90
        onCurrentIndexChanged: {
            root.replace(root.currentItem, root.children[(currentIndex+1)])
            
            console.log(currentIndex, root.currentItem.objectName)
            for(var i = 0; i < count; i++) {
                var t = root.children[i+1]
                if(i !== currentIndex) {                   
                    t.z = (t.z - 1) < 0 ? 0 : t.z - 1
                }
                else {
                    t.z = 5
                }
                
                console.log("move", i, t.objectName, t.z, t.states.length)                
            }
        }
    }
    
    Component.onCompleted: {
        pageChange.currentIndex = 0
        
        for(var i = 1; i < children.length; i++) {
            children[i].width = Qt.binding(function() { return root.width * 0.9})
            children[i].height = Qt.binding(function() { return root.height * 0.9 })
            children[i].StackView.visible = true
            children[i].z = (5 - i) < 1 ? 0 : (5-i)
            
            children[i].states = Qt.binding( function(){ return [ 
                        State {
                            name: "background"
                            when: zChanged && z < 5
                            PropertyChanges {
                                target: children[i]
                                x: z*(root.width*0.1/root.c) < 0 ? 0 : z * (root.width*0.1/root.c)
                            }
                            PropertyChanges {
                                target: children[i]
                                y: z*(root.height*0.1/root.c) < 0 ? 0 : z * (root.height*0.1/root.c)
                            }
                        }
                        , 
                        State {
                            name: "front"
                            when: zChanged && z === 5
                            
                            PropertyChanges {
                                target: children[i]
                                x: root.width*0.1
                            }
                            PropertyChanges {
                                target: children[i]
                                y: root.height*0.1
                            }
                        }] })
            
        }
    }
    
    //    State {
    //        name: "background"
    //        when: zChanged && z < 5
    //        property var item: null
    //        PropertyChanges {
    //            target: parent.item
    //            x: z*(root.width*0.1/root.c) < 0 ? 0 : z * (root.width*0.1/root.c)
    //        }
    //        PropertyChanges {
    //            target: parent.item
    //            y: z*(root.height*0.1/root.c) < 0 ? 0 : z * (root.height*0.1/root.c)
    //        }
    //    }
    State {
        id: front
        name: "front"
        when: zChanged && z === 5
        property var item: null
        PropertyChanges {
            target: parent.item
            x: root.width*0.1
        }
        PropertyChanges {
            target: parent.item
            y: root.height*0.1
        }
    }
    
    initialItem: children[1]
    
    property int c: Math.min(pageChange.count - 1, 5)
    
    //    replaceEnter: Transition {
    //        ParallelAnimation {
    //            PropertyAnimation {
    //                properties: "state"
    //                to: front
    //            }
    //            PropertyAnimation {
    //                properties: "z"
    //                to: 5
    //            }
    //            PropertyAnimation {
    //                properties: "opacity"
    //                to: 1
    //            }
    //        }
    //    }
    
    //    replaceExit: Transition {
    //        id: out
    //        ParallelAnimation {
    //            PropertyAnimation {
    //                properties: "opacity"
    //                to: 1
    //            }
    //            PropertyAnimation {
    //                properties: "state"
    //                to: background
    //            }
    
    //        }
    //    }
    
    //        property real offset: 10
    //        width: 100; height: 100
    
    //        initialItem: Component {
    //            id: page
    //            Rectangle {
    //                property real pos: StackView.index * root.offset
    //                property real hue: Math.random()
    //                color: Qt.hsla(hue, 0.5, 0.8, 0.6)
    //                border.color: Qt.hsla(hue, 0.5, 0.5, 0.9)
    //                StackView.visible: true
    //            }
    //        }
    
    //        replaceEnter: Transition {
    //            id: pushEnter
    //            ParallelAnimation {
    //                PropertyAction { property: "x"; value: pushEnter.ViewTransition.item.pos }
    //                NumberAnimation { properties: "y"; from: pushEnter.ViewTransition.item.pos + root.offset; to: pushEnter.ViewTransition.item.pos; duration: 400; easing.type: Easing.OutCubic }
    //                NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 400; easing.type: Easing.OutCubic }
    //            }
    //        }
    //        popExit: Transition {
    //            id: popExit
    //            ParallelAnimation {
    //                PropertyAction { property: "x"; value: popExit.ViewTransition.item.pos }
    //                NumberAnimation { properties: "y"; from: popExit.ViewTransition.item.pos; to: popExit.ViewTransition.item.pos + root.offset; duration: 400; easing.type: Easing.OutCubic }
    //                NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 400; easing.type: Easing.OutCubic }
    //            }
    //        }
    
    //        replaceExit: Transition {
    //            id: pushExit
    //            PropertyAction { property: "x"; value: pushExit.ViewTransition.item.pos }
    //            PropertyAction { property: "y"; value: pushExit.ViewTransition.item.pos }
    //        }
    //        popEnter: Transition {
    //            id: popEnter
    //            PropertyAction { property: "x"; value: popEnter.ViewTransition.item.pos }
    //            PropertyAction { property: "y"; value: popEnter.ViewTransition.item.pos }
    //        }
}
