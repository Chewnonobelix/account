pragma Singleton
import QtQuick 2.0
import Account 1.0

QtObject {
    property ListModel freqModel: ListModel {
        id: freqModel

        function findIndex(role) {
            for(var i = 0; i < count; i++) {
                if(role === get(i).role) {
                    return i
                }
            }

            return 0
        }

        ListElement {
            name: qsTr("Unique")
            role: Account.Unique
        }

        ListElement {
            name: qsTr("Day")
            role: Account.Day
        }
        ListElement {
            name: qsTr("Week")
            role: Account.Week
        }
        ListElement {
            name: qsTr("Month")
            role: Account.Month
        }
        ListElement {
            name: qsTr("Quarter")
            role: Account.Quarter
        }
        ListElement {
            name: qsTr("Year")
            role: Account.Year
        }
    }
    
    property ListModel entryTypeModel: ListModel {
        function findIndex(role) {
            for(var i = 0; i < count; i++) {
                if(role === get(i).role) {
                    return i
                }
            }

            return 0
        }

        ListElement {
            name: qsTr("Money")
            role: Account.Money
        }

        ListElement {
            name: qsTr("CB")
            role: Account.CB
        }
        ListElement {
            name: qsTr("Transfert")
            role: Account.Transfert
        }
        ListElement {
            name: qsTr("Cheque")
            role: Account.Cheque
        }
    }

    property ListModel typeModel: ListModel {
        id: typeModel

        function find (role) {
             return role === "income" ? 0 : 1
        }

        ListElement {name: qsTr("Income"); type: "income"}
        ListElement {name: qsTr("Outcome"); type: "outcome"}
    }
}
