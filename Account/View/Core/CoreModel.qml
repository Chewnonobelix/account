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
						role: Account.FrequencyEnum.Unique
        }

        ListElement {
            name: qsTr("Day")
						role: Account.FrequencyEnum.Day
        }
        ListElement {
            name: qsTr("Week")
						role: Account.FrequencyEnum.Week
        }
        ListElement {
            name: qsTr("Month")
						role: Account.FrequencyEnum.Month
        }
        ListElement {
            name: qsTr("Quarter")
						role: Account.FrequencyEnum.Quarter
        }
        ListElement {
            name: qsTr("Year")
						role: Account.FrequencyEnum.Year
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

        function findText(role) {
            var index = findIndex(role)
            return get(index).name
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

				function findIndex (role) {
             return role === Account.Income ? 0 : 1
        }

				function findText(role) {
						var index = findIndex(role)
						return get(index).name
				}

        ListElement {name: qsTr("Income"); type: Account.Income}
        ListElement {name: qsTr("Outcome"); type: Account.Outcome}
    }
}
