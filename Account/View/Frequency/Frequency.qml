import QtQuick 2.12
import QtQuick.Controls 2.5

import "../Core" as C
import "../Style" as S

Item {

    S.AccountStyle {
        id: pageStyle
    }

    ListModel {
        id: pastModel
        objectName: "pastModel"
    }

    ListModel {
        id: nextModel
        objectName: "nextModel"
    }

    ListModel {
        id: occurancyModel
        objectName: "occurancyModel"
        
        ListElement {
            role: 1
            name: qsTr("Day")
        }
        ListElement {
            role: 2
            name: qsTr("Week")
        }
        ListElement {
            role: 3
            name: qsTr("Month")
        }
        ListElement {
            role: 4
            name: qsTr("Quarter")
        }
        ListElement {
            role: 5
            name: qsTr("Year")
        }
    }

//    ListView {
//        id: pastView
//        model: pastModel
//    }

//    ListView {
//        id: nextView
//        model: nextModel
//    }

    ComboBox {
        id: occurancy
        objectName: "occurancy"
        model: occurancyModel
        textRole: "name"
        anchors.left: parent.left
    }

    CheckBox {
        id: unlimited
        objectName: "unlimited"
        anchors.left: occurancy.right
        anchors.top: parent.top
        text: qsTr("Unlimited")
    }

    C.CalendarButton {
        id: end
        objectName: "end"
        anchors.left: unlimited.right
        enabled: !unlimited.checked
        anchors.top: parent.top
    }
    
    Button {
        id: save
        objectName: "save"
        text: qsTr("Save")
        anchors.left: end.right
        anchors.leftMargin: 10
        anchors.top: end.top
        
        background: Rectangle {
            gradient: parent.pressed ? pageStyle.goldButton : pageStyle.darkGoldButton
        }
    }

}
