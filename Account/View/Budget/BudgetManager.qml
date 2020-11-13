import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
import QtQuick.controls.Styles 1.4
import QtQuick.Controls 2.15
import "../Style"

import Account.Model 1.0
import Account 1.0

Rectangle {
	id: root

	property bool blocked: false

	color: "transparent"

	ReferenceView {
		objectName: "reference"
		anchors.centerIn: parent
		height: width * 1.33
		width: 150 * 1.5
	}

	ListModel {
		id: categoryModel

		function getIndex(name) {
			var ret = -1;

			for(var i = 0; i < count; i++) {
				if(get(i).catName === name)
					ret = i;
			}

			return ret
		}


		onCountChanged: catView.currentIndex = -1
	}

	Connections {
		target: _budget

		function onAddCat(cat) {
			categoryModel.append(cat)
		}

		function onClearCat() {
			categoryModel.clear()
		}

		function onAddTarget2(target2) {
			targetModel.append(target2)
		}

		function onClearTarget() {
			targetModel.clear()
		}

		function onAddSub(sub) {
			subModel.append(sub)
		}

		function onClearSub() {
			subModel.clear()
		}

		function onBlocked(block) {
			root.blocked = block
		}

		function onSelectCat(select) {
			catView.currentIndex = categoryModel.getIndex(select)        }
	}

	ListModel {
		id: targetModel
	}

	ListModel {
		id: subModel
	}

	RowLayout {
		anchors.fill: parent

		spacing: width * 0.02

		ListView {
			id: outcomeListView
			model: BudgetModel {
				type: Account.TypeEnum.Outcome
			}

			Layout.preferredHeight:  root.height * .85
			Layout.preferredWidth: root.width * .30
			Layout.alignment: Qt.AlignTop

			header: AccountHeader {
				width: outcomeListView.width
				height: outcomeListView.height * .15
				text: qsTr("Outcome")
			}

			delegate: AccountBackground {
				invisible: true
				Component.onCompleted: {
					console.log(has, idBudget, category, name )
				}
				width: outcomeListView.width
				height: outcomeListView.height * .10


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
						if(mouse.button == Qt.RightButton)
							catMenu.open()
					}

				}
			}
		}
		//		ListView {
		//			id: catView
		//			model: categoryModel

		//			Layout.preferredHeight:  root.height * .85
		//			Layout.preferredWidth: root.width * .30
		//			Layout.alignment: Qt.AlignTop

		//			section.property: "type"
		//			section.criteria: ViewSection.FullString
		//			section.delegate: Rectangle {
		//				gradient: AccountStyle.goldHeader
		//				width: catView.width
		//				height: 30
		//				Text {
		//					anchors.fill: parent
		//					text: section
		//					horizontalAlignment: Text.AlignHCenter
		//					verticalAlignment: Text.AlignVCenter
		//					fontSizeMode: Text.Fit
		//					font.family: AccountStyle.title.name
		//					font.pixelSize: AccountStyle.title.size2
		//				}
		//			}

		//			section.labelPositioning: ViewSection.InlineLabels


		//			currentIndex: -1
		//			clip: true

		//			delegate: Rectangle {
		//				width: catView.width
		//				height: 40
		//				id: delItem

		//				gradient: categoryModel.getIndex(catName) === catView.currentIndex ? AccountStyle.calSelect : AccountStyle.unselectView
		//				AccountLabel {
		//					id: catDisplay
		//					color: has ? "black" : "red"
		//					anchors.centerIn: parent
		//					text: catName
		//				}

		//				MouseArea {
		//					anchors.fill: parent
		//					hoverEnabled: true
		//					cursorShape: Qt.PointingHandCursor
		//					acceptedButtons: Qt.LeftButton | Qt.RightButton
		//					onClicked: {
		//						if(mouse.button === Qt.LeftButton) {
		//							catView.currentIndex = index

		//							if(categoryModel.get(catView.currentIndex).has)
		//								_budget.getTarget(catName)
		//						}
		//						else {
		//							catMenu.popup()
		//						}
		//					}
		//				}

		//				Menu {
		//					id: catMenu

		//					height: delItem.height

		//					delegate: MenuItem {
		//						font.family: AccountStyle.core.name
		//						font.pixelSize: AccountStyle.core.size

		//						height: catMenu.height
		//						background: Rectangle {
		//							anchors.fill: parent
		//							gradient: parent.pressed ? AccountStyle.darkGoldButton : AccountStyle.goldButton
		//						}
		//					}

		//					Action {
		//						//TODO
		//						id: addBudget
		//						text:  has ? qsTr("Remove budget") : qsTr("Add budget")
		//						onTriggered:  _budget.addBudget(catName)
		//					}

		//				}
		//			}


		//			Rectangle {
		//				anchors.fill: parent
		//				color: "transparent"
		//				border.color: "darkseagreen"
		//			}
		//		}

		Rectangle {
			border.color: "darkseagreen"
			color: "transparent"
			Layout.preferredHeight: root.height * .85
			Layout.preferredWidth: root.width * .30
			Layout.alignment: Qt.AlignTop
			visible: catView.currentIndex !== -1 && categoryModel.get(catView.currentIndex).has

			ListView {
				id: targetView
				objectName: "targetView"
				anchors.fill: parent

				visible: catView.currentIndex !== -1 && categoryModel.get(catView.currentIndex).has

				ToolTip.text: catView.currentIndex !== -1 ? categoryModel.get(catView.currentIndex).catName + " " + qsTr("target list") : ""
				ToolTip.delay: 500
				ToolTip.timeout: 1000
				ToolTip.visible: budgetArea.containsMouse && catView.currentIndex !== -1


				model: targetModel
				currentIndex: -1
				clip: true
				spacing: height * 0.02

				header: AccountHeader {
					text: qsTr("Target")
					width: targetView.width
					height: targetView.height * .07
				}

				onCurrentIndexChanged: {
					var temp = currentIndex !== - 1 ? Qt.formatDate(targetModel.get(currentIndex).date, "dd-MM-yyyy") : ""
					_budget.showTarget(categoryModel.get(catView.currentIndex).catName, temp, currentIndex === -1)
				}

				Component {
					id: compRemoveAction
					Action {
						id: removeAction
						text:  qsTr("Remove target")

						onTriggered: {
							_budget.removeTarget(categoryModel.get(catView.currentIndex).catName, Qt.formatDate(targetModel.get(targetView.currentIndex).date, "dd-MM-yyyy"))
						}
					}
				}

				Menu {
					id: targetItemMenu

					height: count * 20

					delegate: MenuItem {
						font.family: AccountStyle.core.name
						font.pixelSize: AccountStyle.core.size
						height: 20
						background: Rectangle {
							gradient: parent.highlighted ? AccountStyle.darkGoldButton : AccountStyle.goldButton
						}
					}


					Action {
						id: addSubTarget
						text: qsTr("Add target")
						onTriggered: _budget.addTarget(categoryModel.get(catView.currentIndex).catName)
					}


				}

				delegate: Rectangle {
					width: targetView.width
					height: 40

					border.color: "gray"
					gradient: targetView.currentIndex === index ? AccountStyle.calSelect : AccountStyle.backgroundGradient
					AccountLabel {
						MouseArea {
							anchors.fill: parent
							acceptedButtons: Qt.LeftButton | Qt.RightButton
							z:5
							onClicked: {
								targetView.currentIndex = targetView.currentIndex === index ? -1 :  index
								if(mouse.button === Qt.RightButton && targetItemMenu.count === 1) {
									targetView.currentIndex = index
									targetItemMenu.addAction(compRemoveAction.createObject())
									targetItemMenu.popup()
								}
							}
						}

						id: targetText
						anchors.fill: parent

						text: qsTr("Date") + ": " + Qt.formatDate(date, "dd-MM-yyyy") + "\n" + qsTr("Target") + ": " + target + Qt.locale().currencySymbol(Locale.CurrencySymbol)
					}

				}




				MouseArea {
					id: budgetArea
					anchors.fill: parent
					acceptedButtons: Qt.RightButton
					hoverEnabled: true
					z: -1


					onClicked: {
						targetView.currentIndex = targetView.indexAt(mouse.x, mouse.y)
						if (targetView.currentIndex === -1 && targetItemMenu.count === 2){
							targetItemMenu.takeAction(1)
						}

						targetItemMenu.popup()
					}
				}
			}
		}

		ListView {
			id: subView

			Layout.preferredHeight: root.height * .85
			Layout.preferredWidth: root.width * .30
			Layout.alignment: Qt.AlignTop

			visible: targetView.visible
			model: subModel
			clip: true
			spacing: height * 0.02

			Rectangle {
				anchors.fill: parent
				color: "transparent"
				border.color: "darkseagreen"
			}

			header: AccountHeader {
				text: qsTr("All sub budget")
				width: subView.width
				height: subView.height * .07
			}


			delegate: Rectangle {
				height: 60
				width: subView.width
				gradient: AccountStyle.backgroundGradient
				border.color: "gray"


				Column {
					anchors.fill: parent
					AccountLabel {
						width: parent.width
						height: 25
						text: Qt.formatDate(begin, "dd-MM-yyyy") + " -> " + Qt.formatDate(end, "dd-MM-yyyy")
					}

					BudgetViewItem {
						anchors.bottomMargin: 5
						clip: true
						height: 30
						width: parent.width
						to: target
						realValue: current
						title: cat
					}
				}
			}
		}
	}
}

/*##^## Designer {
		D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
