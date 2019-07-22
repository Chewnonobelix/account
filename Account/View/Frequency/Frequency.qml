import QtQuick 2.12
import QtQuick.Controls 2.5

Item {

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

    ListView {
        id: pastView
        model: pastModel
    }

    ListView {
        id: nextView
        model: nextModel
    }

    ComboBox {
        id: occurancy
        model: occurancyModel
        textRole: "name"
    }
}
