import QtQuick 2.15
import QtQuick.Controls 2.15

import "../Style"
import Account 1.0
import Account.Model 1.0

ListView {
	id: root

	property string title: qsTr("Outcome")
	property var type: Account.TypeEnum.Outcome

	model: BudgetModel {
		type: root.type
	}

	header: AccountHeader {
		width: root.width
		height: root.height * .15
		text: root.title
	}

	highlight: Rectangle{
		gradient: AccountStyle.calSelect
	}

	delegate: AccountBackground {
		invisible: true
		Component.onCompleted: {
			console.log("Item budget", has, idBudget, category, name )
		}
		width: root.width
		height: root.height * .10


		AccountLabel {
			anchors.fill: parent
			text: name
			color: has ? "green" : "red"
		}

		Menu {
			id: catMenu

			height: parent.height

			delegate: MenuItem {
				font.family: AccountStyle.core.name
				font.pixelSize: AccountStyle.core.size

				height: catMenu.height
				background: Rectangle {
					anchors.fill: parent
					gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
				}
			}

			Action {
				//TODO
				id: addBudget
				text:  has ? qsTr("Remove budget") : qsTr("Add budget")
				onTriggered:  has ? _budget.removeBudget(idBudget) : _budget.addBudget(category, outcomeListView.model.type)
			}

		}

		MouseArea {
			anchors.fill: parent
			cursorShape: Qt.PointingHandCursor
			acceptedButtons: Qt.LeftButton | Qt.RightButton

			onClicked: {
				console.log(index)
				if(mouse.button == Qt.RightButton)
					catMenu.open()
				else if(mouse.button == Qt.LeftButton && root.currentIndex !== index)
					root.currentIndex = index
				else
					root.currentIndex = -1
			}
		}
	}
}
