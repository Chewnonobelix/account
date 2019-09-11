import QtQuick 2.12
import QtQuick.Window 2.12

import "../Style"
import "../Core"

Window {
    AccountStyle {
        id: pageStyle
    }

    ListView {
        id: frequencyList
    }
    
    Frequency {
        id: reference
    }
    
}
