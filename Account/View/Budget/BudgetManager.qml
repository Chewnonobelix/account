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

	Connections {
		target: _budget
	}

	property var currentBudget: null
	RowLayout {
		anchors.fill: parent

		spacing: width * 0.02

		Column {
			Layout.preferredHeight:  root.height * .85
			Layout.preferredWidth: root.width * .30
			Layout.alignment: Qt.AlignTop
			clip: true
			BudgetList  {
				id: outcomeList

				title: qsTr("Outcome")
				type: Account.TypeEnum.Outcome
				width: parent.width
				height: parent.height / 2
				clip: true
			}
			BudgetList  {
				id: incomeList

				title: qsTr("Income")
				type: Account.TypeEnum.Income
				width: parent.width
				height: parent.height / 2

				clip: true
			}
		}

		Rectangle {
			border.color: "darkseagreen"
			color: "transparent"
			Layout.preferredHeight: root.height * .85
			Layout.preferredWidth: root.width * .30
			Layout.alignment: Qt.AlignTop
			visible:  outcomeList.currentIndex !== -1

			ListView {
				id: targetView
				objectName: "targetView"
				anchors.fill: parent

				visible: outcomeList.currentIndex !== -1

//				ToolTip.text: outcomeList.currentIndex !== -1 ? categoryModel.get(catView.currentIndex).catName + " " + qsTr("target list") : ""
				ToolTip.delay: 500
				ToolTip.timeout: 1000
//				ToolTip.visible: budgetArea.containsMouse && catView.currentIndex !== -1


				//				model: targetModel
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
			//			model: subModel
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
