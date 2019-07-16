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
    }
}
