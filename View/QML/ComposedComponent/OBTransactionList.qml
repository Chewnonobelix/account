import QtQuick
import QtQuick.Controls

import OpenAccount.Model

import "../Style" as Style
import "../Components" as Comp

// Scrollable, paginated list of transactions, one TransactionRow per row.
// `model` takes an AccountTransactionFilterProxyModel (or its
// TransactionListModel source directly) — anything exposing the
// "transaction" role. Internally windows that model down to pageSize rows
// (100 by default) via PageFilterProxyModel, with a Pager below the list to
// move between pages. Each row manages its own selection highlight
// independently, same as a standalone TransactionRow.
Item {
    id: root

    property AccountTransactionFilterProxyModel model: null
    property int pageSize: 100
    // Items on the current page; see totalCount for the full model size.
    property alias count: listView.count
    property alias currentIndex: listView.currentIndex
    property alias currentPage: pageProxy.currentPage
    property alias pageCount: pageProxy.pageCount
    readonly property int totalCount: root.model ? root.model.count : 0

    implicitWidth: Style.OBConstants.widthBig * 2
    implicitHeight: Style.OBConstants.heightMedium * 6

    PageFilterProxyModel {
        id: pageProxy
        pageSize: root.pageSize
        sourceModel: root.model
    }

    // Pager mutates its own currentPage imperatively on click (button
    // handlers assign root.currentPage directly), which would permanently
    // break a plain `currentPage: pageProxy.currentPage` binding the first
    // time it's clicked. Syncing both directions through signal handlers
    // instead keeps pager and pageProxy in step without that binding loss.
    Pager {
        id: pager
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        maxPage: pageProxy.pageCount
        onCurrentPageChanged: pageProxy.currentPage = pager.currentPage
    }

    Connections {
        target: pageProxy
        function onCurrentPageChanged() {
            pager.currentPage = pageProxy.currentPage
        }
    }

    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: pager.top
        anchors.bottomMargin: Style.OBConstants.verticalSpacing / 2
        radius: Style.OBConstants.borderRadius
        border.width: Style.OBConstants.borderWidth
        border.color: Style.OBTheme.palette.outline
        gradient: Style.OBStyle.background
        clip: true

        ListView {
            id: listView
            anchors.fill: parent
            clip: true
            model: pageProxy
            boundsBehavior: Flickable.StopAtBounds

            ScrollBar.vertical: Comp.OBScrollBar {}

            delegate: TransactionRow {
                width: listView.width
                transaction: model.transaction
            }
        }
    }
}
