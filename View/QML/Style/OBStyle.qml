pragma Singleton

import QtQuick

QtObject {
    id: root

    readonly property OBPalette colors: OBTheme.palette

    property Gradient goldOut: Gradient {
        GradientStop {
            position: 0.0
            color: root.colors.outlineStrong
        }
        GradientStop {
            position: 0.1
            color: root.colors.outline
        }
        GradientStop {
            position: 0.25
            color: root.colors.accent
        }
        GradientStop {
            position: 0.4
            color: root.colors.accentWarm
        }
        GradientStop {
            position: 0.48
            color: root.colors.accentHighlight
        }
        GradientStop {
            position: 0.5
            color: root.colors.surfaceRaised
        }
        GradientStop {
            position: 0.52
            color: root.colors.accentHighlight
        }
        GradientStop {
            position: 0.6
            color: root.colors.accentWarm
        }
        GradientStop {
            position: 0.75
            color: root.colors.accent
        }
        GradientStop {
            position: 0.9
            color: root.colors.outline
        }
        GradientStop {
            position: 1.0
            color: root.colors.outlineStrong
        }
    }

    property Gradient goldIn: Gradient {
        GradientStop {
            position: 0.0
            color: root.colors.surfaceRaised
        }
        GradientStop {
            position: 0.08
            color: root.colors.accentHighlight
        }
        GradientStop {
            position: 0.18
            color: root.colors.accentWarm
        }
        GradientStop {
            position: 0.35
            color: root.colors.accentStrong
        }
        GradientStop {
            position: 0.50
            color: root.colors.accent
        }
        GradientStop {
            position: 0.65
            color: root.colors.outlineStrong
        }
        GradientStop {
            position: 0.82
            color: root.colors.accentWarm
        }
        GradientStop {
            position: 0.92
            color: root.colors.accentHighlight
        }
        GradientStop {
            position: 1.0
            color: root.colors.surfaceRaised
        }
    }

    property Gradient silver: Gradient {
        GradientStop {
            position: 0.0
            color: root.colors.disabledDark
        }
        GradientStop {
            position: 0.2
            color: root.colors.disabled
        }
        GradientStop {
            position: 0.4
            color: root.colors.disabledLight
        }
        GradientStop {
            position: 0.5
            color: root.colors.backgroundEnd
        }
        GradientStop {
            position: 0.6
            color: root.colors.disabledLight
        }
        GradientStop {
            position: 0.8
            color: root.colors.disabled
        }
        GradientStop {
            position: 1.0
            color: root.colors.disabledDark
        }
    }

    property Gradient positive: Gradient {
        GradientStop {
            position: 0.0
            color: root.colors.success
        }
        GradientStop {
            position: 0.15
            color: root.colors.successMuted
        }
        GradientStop {
            position: 0.85
            color: root.colors.successMuted
        }
        GradientStop {
            position: 1.0
            color: root.colors.success
        }
    }

    property Gradient negative: Gradient {
        GradientStop {
            position: 0.0
            color: root.colors.danger
        }
        GradientStop {
            position: 0.15
            color: root.colors.dangerMuted
        }
        GradientStop {
            position: 0.85
            color: root.colors.dangerMuted
        }
        GradientStop {
            position: 1.0
            color: root.colors.danger
        }
    }

    property Gradient selected: Gradient {
        GradientStop {
            position: 0.0
            color: root.colors.selection
        }
        GradientStop {
            position: 0.5
            color: root.colors.selectionContrast
        }
        GradientStop {
            position: 1.0
            color: root.colors.selection
        }
    }

    property Gradient background: Gradient {
        GradientStop {
            position: 0.0
            color: root.colors.backgroundStart
        }
        GradientStop {
            position: 1.0
            color: root.colors.backgroundEnd
        }
    }

    property Gradient header: Gradient {
        GradientStop {
            position: 0.0
            color: root.colors.headerStart
        }
        GradientStop {
            position: 0.08
            color: root.colors.outline
        }
        GradientStop {
            position: 0.25
            color: root.colors.headerMid
        }
        GradientStop {
            position: 0.47
            color: root.colors.accentWarm
        }
        GradientStop {
            position: 0.68
            color: root.colors.accentStrong
        }
        GradientStop {
            position: 0.82
            color: root.colors.surfaceSunken
        }
        GradientStop {
            position: 0.92
            color: root.colors.surface
        }
        GradientStop {
            position: 1.0
            color: root.colors.headerEnd
        }
    }
    property Gradient radioSelected: Gradient {
        GradientStop {
            position: 0.0
            color: root.colors.radioCenter
        }
        GradientStop {
            position: 1.0
            color: root.colors.textPrimary
        }
    }

    property Gradient black: Gradient {
        GradientStop {
            position: 0.0
            color: root.colors.textPrimary
        }
        GradientStop {
            position: 1.0
            color: root.colors.textPrimary
        }
    }

    readonly property color accent: root.colors.accent
    readonly property color textPrimary: root.colors.textPrimary
    readonly property color textSecondary: root.colors.textSecondary
    readonly property color textDisabled: root.colors.textDisabled
    readonly property color backgroundStart: root.colors.backgroundStart
    readonly property color backgroundEnd: root.colors.backgroundEnd
    readonly property color success: root.colors.success
    readonly property color danger: root.colors.danger
}
