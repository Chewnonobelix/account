import QtQuick

QtObject {
    property color backgroundStart
    property color backgroundEnd

    property color surface
    property color surfaceRaised
    property color surfaceSunken

    property color outline
    property color outlineStrong
    property color accent
    property color accentWarm
    property color accentStrong
    property color accentHighlight
    // Text/foreground drawn on top of the gold gradients (goldOut/goldIn)
    // or the selection highlight, both of which run light-toned through
    // most of their span in every theme; keep this dark regardless of mode.
    property color accentContrast

    property color headerStart
    property color headerMid
    property color headerEnd

    property color selection
    property color selectionContrast

    property color textPrimary
    property color textSecondary
    property color textDisabled

    property color disabled
    property color disabledDark
    property color disabledLight

    property color success
    property color successMuted
    property color danger
    property color dangerMuted

    property color radioCenter
}
