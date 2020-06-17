import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

StackView {
    id: root    
    
    PageIndicator {
        anchors.right: root.right
        anchors.verticalCenter: parent.verticalCenter
        id: pageChange
        count: root.children.length - 1
        currentIndex: 0
        z: root.children.length+10
        interactive: true
        
        
        rotation: 90
        onCurrentIndexChanged: {
            var l = []
            for(var i = 0; i < count; i++) {
                var t = root.children[i+1]
                if(t.z === 5) {
                    t.z = root.children[currentIndex+1].z
                    root.children[currentIndex+1].z = 5
                }
            }
            root.replace(root.currentItem, root.children[(currentIndex+1)])
        }
    }

    Component.onCompleted: {
        for(var i = 1; i < children.length-1; i++) {
            var temp = children[i]
            temp.width = Qt.binding(function() { return root.width * 0.9})
            temp.height = Qt.binding(function() { return root.height * 0.9 })
            temp.StackView.visible = true
            temp.z = (5 - i + 1) < 1 ? 0 : (5-i + 1)

            temp.opacity = 0.5
            temp.x = Qt.binding(function() { return (root.width*0.08/5) * (this.z) })
            temp.y = Qt.binding(function() { return (root.height*0.08/5) * (this.z) })
        }
        root.replace(initialItem, root.children[1])
    }
    
    initialItem: Rectangle {
        width: root.width * 0.9
        height: root.height * 0.9

        opacity: 0
        x: (root.width*0.08/5) * (this.z)
        y: (root.height*0.08/5) * (this.z)
    }

    replaceEnter: Transition {
        id: enter
        ParallelAnimation {
            XAnimator {
                to: root.width*0.08
            }
            YAnimator {
                to: root.height*0.08
            }
            PropertyAnimation {
                properties: "opacity"
                to: 1
            }
        }
    }
    
    replaceExit: Transition {
        id: out
        ParallelAnimation {
            PropertyAnimation {
                properties: "opacity"
                to: 0.5
            }
            XAnimator {
                to: (root.width*0.08/5) * out.ViewTransition.item.z
            }
            YAnimator {
                to: (root.height*0.08/5) * out.ViewTransition.item.z
            }
        }
    }
}
