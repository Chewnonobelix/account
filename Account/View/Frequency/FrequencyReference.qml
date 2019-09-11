import QtQuick 2.13

import "../Style" as S
import "../Core" as C

Item {
    S.AccountStyle {
        id: pageStyle
    }
    
    C.EntryEdit {
        id: freqRef
        objectName: "freqRef"
    }

}
