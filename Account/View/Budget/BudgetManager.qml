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
		id: reference
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
				onCurrentIndexChanged: if(currentIndex !== -1) incomeList.currentIndex = -1

				title: qsTr("Outcome")
				type: Account.TypeEnum.Outcome
				width: parent.width
				height: parent.height / 2
				clip: true

				onCurrentModelChanged: {
					targetModel.currentId = currentModel
					targetModel.budget = _budget.get(currentModel)
				}
			}
			BudgetList  {
				id: incomeList

				onCurrentIndexChanged: if(currentIndex !== -1) outcomeList.currentIndex = -1

				title: qsTr("Income")
				type: Account.TypeEnum.Income
				width: parent.width
				height: parent.height / 2

				clip: true

				onCurrentModelChanged: {
					targetModel.currentId = currentModel
					targetModel.budget = _budget.get(currentModel)
				}
			}
		}

		Rectangle {
			border.color: "darkseagreen"
			color: "transparent"
			Layout.preferredHeight: root.height * .85
			Layout.preferredWidth: root.width * .30
			Layout.alignment: Qt.AlignTop
			visible:  (outcomeList.currentIndex !== -1 || incomeList.currentIndex !== -1 )
			id: rect
			onVisibleChanged: {
			}

			ListView {
				id: targetView
				objectName: "targetView"
				anchors.fill: parent

				//				ToolTip.text: outcomeList.currentIndex !== -1 ? categoryModel.get(catView.currentIndex).catName + " " + qsTr("target list") : ""
				ToolTip.delay: 500
				ToolTip.timeout: 1000
				ToolTip.visible: budgetArea.containsMouse && (incomeList.currentIndex !== -1 || outcomeList.currentIndex !== -1)

				model: TargetListModel {
					id: targetModel

					onBudgetChanged: {
						rect.visible = isValid
						if(isValid)
							subView.model = allSubs()
					}
				}

				currentIndex: -1
				clip: true
				spacing: height * 0.02

				headerPositioning: ListView.OverlayHeader
				header: AccountHeader {
					text: qsTr("Target")
					width: targetView.width
					height: targetView.height * .07
					z:5
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
					width: targetView.width * 0.40
					z: 10
					delegate: MenuItem {
						font.family: AccountStyle.core.name
						font.pixelSize: AccountStyle.core.size
						height: 20
						background: Rectangle {
							gradient: parent.highlighted ? AccountStyle.darkGoldButton : AccountStyle.goldButton
						}
					}


					onOpened: console.log("bite", count)

					Action {
						id: addSubTarget
						text: qsTr("Add target")
						onTriggered: _budget.addTarget(targetModel.currentId)
					}


				}

				delegate: Rectangle {
					width: targetView.width
					height: targetView.height * 0.10
					clip: true

					border.color: "gray"
					gradient: targetView.currentIndex === index ? AccountStyle.calSelect : AccountStyle.backgroundGradient
					AccountLabel {
						MouseArea {
							anchors.fill: parent
							acceptedButtons: Qt.LeftButton | Qt.RightButton
							z:5
							onClicked: {
								targetView.currentIndex = targetView.currentIndex === index ? -1 :  index
								subView.model = subs
								subView.selectedDate = Qt.formatDate(date, "dd-MM-yyyy")
								if(mouse.button === Qt.RightButton) {
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

			property string selectedDate
			header: AccountHeader {
				text: qsTr("Sub budget") + ": " + (targetView.currentIndex === - 1 ? qsTr("All") : subView.selectedDate)
				width: subView.width
				height: subView.height * .07
				z: 5
			}

			headerPositioning: ListView.OverlayHeader

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
						text: Qt.formatDate(modelData.begin, "dd-MM-yyyy") + " -> " + Qt.formatDate(modelData.end, "dd-MM-yyyy")
					}

					BudgetViewItem {
						anchors.bottomMargin: 5
						clip: true
						height: 30
						width: parent.width
						to: modelData.target
						realValue: modelData.current
						title: targetModel.name
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
