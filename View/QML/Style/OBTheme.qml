pragma Singleton

import QtQuick

QtObject {
    enum Mode {
        Day,
        Night
    }

    property int mode: OBTheme.Mode.Day
    readonly property OBDayPalette dayPalette: OBDayPalette {}
    readonly property OBNightPalette nightPalette: OBNightPalette {}

    readonly property bool isDay: mode === OBTheme.Mode.Day
    readonly property bool isNight: mode === OBTheme.Mode.Night
    readonly property OBPalette palette: isNight ? nightPalette : dayPalette
}
