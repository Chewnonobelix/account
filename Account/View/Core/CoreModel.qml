import QtQuick 2.0

QtObject {
    property ListModel freqModel: ListModel {
        id: freqModel
        ListElement {
            name: qsTr("Day")
            role: 1
        }
        ListElement {
            name: qsTr("Week")
            role: 2
        }
        ListElement {
            name: qsTr("Month")
            role: 3
        }
        ListElement {
            name: qsTr("Quarter")
            role: 4
        }
        ListElement {
            name: qsTr("Year")
            role: 5
        }
    }
    
    property ListModel typeModel: ListModel {
        id: typeModel
        ListElement {name: qsTr("Income"); type: "income"}
        ListElement {name: qsTr("Outcome"); type: "outcome"}
    }
}
