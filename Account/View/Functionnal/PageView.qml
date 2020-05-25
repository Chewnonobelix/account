import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

StackView {
    id: root

    property int index: 0


    PageIndicator {
        anchors.right: root.right
        width: parent.height
        height: parent.width * 0.05
        id: pageChange
        count: root.children.length - 1
        currentIndex: -1
        z: 10
        interactive: true
        
        background: Rectangle {
            border.color: "gold"
        }

        rotation: -90
        onCurrentIndexChanged: {
            var i = (currentIndex + 1) % (count)
            root.clear()
            while(i !== currentIndex) {
                root.push(root.children[(i+1)])
                i = (i+1)%(count)
            }
            root.push(root.children[(i+1)])
        }
    }
    Component.onCompleted: {
        pageChange.currentIndex = 0
    }
    
    
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
    
//        pushEnter: Transition {
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
    
//        pushExit: Transition {
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
