import QtQuick 2.15
//import QtQuick.Controls 1.4
//import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.15
import "../Style"
import "../Functionnal"

AccountButton {
    id: button
    text: Qt.formatDate(new Date(), "dd-MM-yyyy")

    property bool isOpen: pop.opened
    ToolTip.text: qsTr("Select a single date")

    function extern(d) {
        dateAdding.set(d)
        button.text = Qt.formatDate(d, "dd-MM-yyyy")
    }

    onClicked: {
        pop.open()
    }
    
    Popup {
        id: pop
        background: AccountBackground{invisible: true}
        
        ZoomCalendar {
            id: dateAdding
            multiple: false
            onClicked: pop.close()
            height: 250
            width: 250

            onDatesChanged: {
                button.text = Qt.formatDate(selectedDate, "dd-MM-yyyy")
                pop.close()
            }
        }
    }

}
