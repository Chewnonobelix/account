import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQml.Models 2.12

Item {
    ListModel {
        id: budgetModel

        ListElement {
            target: 100
            currentValue: 102
            name: "B1"
        }

        ListElement {
            target: 200
            currentValue: 100
            name: "B2"
        }

        ListElement {
            target: 200
            currentValue: 102
            name: "B3"
        }

        ListElement {
            target: 605
            currentValue: 102
            name: "B4"
        }

        ListElement {
            target: 500
            currentValue: 1102
            name: "B5"
        }
    }

    Column {
        id: budgetList
        anchors.fill: parent

        Repeater {
            model: budgetModel
            delegate: BudgetViewItem {
                to: target
                realValue: currentValue
                title: name
                Component.onCompleted: console.log(name)
            }
        }
    }
}
