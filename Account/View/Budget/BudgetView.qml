import QtQuick 2.15
import QtQml.Models 2.15

import Account.Model 1.0

GridView {
	id: root
	model: _budgetQuickModel

	cellHeight: height * 0.12
	cellWidth:  width * 0.50

	delegate: BudgetViewItem {
		required property string name
		required property double target
		required property double current

		height: root.height * 0.10
		width:  root.width * 0.48

		to: target
		title: name
		realValue: current
	}
}
