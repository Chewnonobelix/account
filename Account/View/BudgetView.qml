import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQml.Models 2.12

Item {

    function add(mapcol) {
        if (col === 0)
            budgetList1.add(map)
        else
            budgetList2.add(map)
    }

    ListModel {
        id: budgetModel1
        //target, currentValue, name,date
    }

    ListModel {
        id: budgetModel2
        //target, currentValue, name,date
    }

    Column {
        id: budgetList1
        anchors.left: parent.left
        width: (parent.width / 2) - 5
        Repeater {
            model: budgetModel1
            delegate: BudgetViewItem {
                to: target
                realValue: currentValue
                title: name
            }
        }
    }

    Column {
        id: budgetList2
        anchors.right: parent.right
        anchors.rightMargin: 5
        width: (parent.width / 2) - 5
        Repeater {
            model: budgetModel2
            delegate: BudgetViewItem {
                to: target
                realValue: currentValue
                title: name
            }
        }
    }
}
