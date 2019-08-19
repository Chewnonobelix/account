import QtQuick 2.12
import QtQuick.Controls 2.5

import "../Core" as C
import "../Style" as S

Item {

    S.AccountStyle {
        id: pageStyle
    }

    ListModel {
        id: pastModel
        objectName: "pastModel"
    }

}
