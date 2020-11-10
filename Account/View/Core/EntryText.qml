import QtQuick 2.15
import QtQuick.Layouts 1.15

import "../Style"

import Account 1.0

Item {
    id: root

    property var model: null

    onModelChanged: {
        if(model) {
            date.data = Qt.formatDate(model.date)
            label.data = model.title
            value.data = model.value
						type.data = model.type === Account.Outcome ? Account.Outcome : Account.Income
            support.data = CoreModel.entryTypeModel.findText(model.support)
            category.data = model.category
        }
    }

    ColumnLayout {
        anchors.fill: parent

        AccountLabel {
            id: date
            property string data

            text: qsTr("Date") + ": " + data
        }
        AccountLabel {
            id: label
            property string data

            text: qsTr("Label") + ": " + data
        }
        AccountLabel {
            id: value
            property string data

            text: qsTr("Value") + ": " + data
        }
        AccountLabel {
            id: category
            property string data

            text: qsTr("Category") + ": " + data
        }
        AccountLabel {
            id: type
            property string data

            text: qsTr("Type") + ": " + data
        }
        AccountLabel {
            id: support
            property string data

            text: qsTr("Support") + ": " + data
        }
    }
}
