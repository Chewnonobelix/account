import QtQuick

import "../Style" as Style
import "../Components" as Comp

// Page navigation control: jump-to-first, previous, next and jump-to-last
// buttons around a "current/max" page indicator. Boundary buttons disable
// themselves automatically on the first/last page.
Item {
    id: root

    property int currentPage: 1
    property int maxPage: 1

    readonly property bool isFirstPage: root.currentPage <= 1
    readonly property bool isLastPage: root.currentPage >= root.maxPage

    implicitWidth: contentRow.implicitWidth
    implicitHeight: Style.OBConstants.heightMedium

    function goToFirstPage() {
        setPage(1)
    }

    function goToPreviousPage() {
        setPage(root.currentPage - 1)
    }

    function goToNextPage() {
        setPage(root.currentPage + 1)
    }

    function goToLastPage() {
        setPage(root.maxPage)
    }

    function setPage(page) {
        root.currentPage = Math.max(1, Math.min(root.maxPage, page))
    }

    Row {
        id: contentRow
        anchors.centerIn: parent
        spacing: Style.OBConstants.horizontalSpacing / 2

        Comp.OBButton {
            id: bigPreviousButton
            text: "«"
            implicitWidth: Style.OBConstants.widthLittle / 2
            enabled: !root.isFirstPage
            onClicked: root.goToFirstPage()
        }

        Comp.OBButton {
            id: previousButton
            text: "‹"
            implicitWidth: Style.OBConstants.widthLittle / 2
            enabled: !root.isFirstPage
            onClicked: root.goToPreviousPage()
        }

        Comp.OBLabel {
            id: pageLabel
            text: root.currentPage + "/" + root.maxPage
            width: Style.OBConstants.widthLittle
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        Comp.OBButton {
            id: nextButton
            text: "›"
            implicitWidth: Style.OBConstants.widthLittle / 2
            enabled: !root.isLastPage
            onClicked: root.goToNextPage()
        }

        Comp.OBButton {
            id: bigNextButton
            text: "»"
            implicitWidth: Style.OBConstants.widthLittle / 2
            enabled: !root.isLastPage
            onClicked: root.goToLastPage()
        }
    }
}
