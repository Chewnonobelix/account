import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQml.Models 2.12

Item {

    function add(map, col) {
        if (col === 0)
            budgetModel1.append(map)
        else
            budgetModel2.append(map)
    }

    function clear() {
        budgetModel1.clear()
        budgetModel2.clear()
    }

//    Rectangle {
//        anchors.fill: parent
//    }

    ListModel {
        id: budgetModel1
        //target, currentValue, name,date
    }

    ListModel {
        id: budgetModel2
        //target, currentValue, name,date
    }

    ListView {
        id: budgetList1
        anchors.left: parent.left
        width: (parent.width / 2) - 5
        model: budgetModel1
        delegate: BudgetViewItem {
            clip: true
            width: budgetList1.width
            to: target
            realValue: currentValue
            title: name
        }
    }

    ListView {
        id: budgetList2
        anchors.left: budgetList1.right
        anchors.leftMargin: 5
        width: (parent.width / 2) - 5
        model: budgetModel2
        delegate: BudgetViewItem {
            clip: true
            width: budgetList2.width
            to: target
            realValue: currentValue
            title: name
        }
    }
}