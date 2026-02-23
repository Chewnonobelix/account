import QtQuick
import QtQuick.Shapes

QtObject {
    property Gradient goldOut: Gradient {
        GradientStop {
            position: 0.0
            color: "#B59410"
        }
        GradientStop {
            position: 0.1
            color: "#C49B16"
        }
        GradientStop {
            position: 0.25
            color: "#D4AF37"
        }
        GradientStop {
            position: 0.4
            color: "#EFC94D"
        }
        GradientStop {
            position: 0.48
            color: "#FCEA74"
        }
        GradientStop {
            position: 0.5
            color: "#FFF9E3"
        }
        GradientStop {
            position: 0.52
            color: "#FCEA74"
        }
        GradientStop {
            position: 0.6
            color: "#EFC94D"
        }
        GradientStop {
            position: 0.75
            color: "#D4AF37"
        }
        GradientStop {
            position: 0.9
            color: "#C49B16"
        }
        GradientStop {
            position: 1.0
            color: "#B59410"
        }

    }

    property Gradient goldIn: Gradient {
        GradientStop {
            position: 0.0
            color: "#FFF9E7"
        }
        GradientStop {
            position: 0.08
            color: "#FCEA74"
        }
        GradientStop {
            position: 0.18
            color: "#F8D653"
        }
        GradientStop {
            position: 0.35
            color: "#FFD700"
        }
        GradientStop {
            position: 0.50
            color: "#D4AF37"
        }
        GradientStop {
            position: 0.65
            color: "#BB8B06"
        }
        GradientStop {
            position: 0.82
            color: "#F8D653"
        }
        GradientStop {
            position: 0.92
            color: "#FCEA74"
        }
        GradientStop {
            position: 1.0
            color: "#FFF9E7"
        }
    }

    property Gradient silver: Gradient {
        GradientStop {
            position: 0.0
            color: "#A0A0A0"
        }
        GradientStop {
            position: 0.2
            color: "#C0C0C0"
        }
        GradientStop {
            position: 0.4
            color: "#E0E0E0"
        }
        GradientStop {
            position: 0.5
            color: "#F8F8F8"
        }
        GradientStop {
            position: 0.6
            color: "#E0E0E0"
        }
        GradientStop {
            position: 0.8
            color: "#C0C0C0"
        }
        GradientStop {
            position: 1.0
            color: "#A0A0A0"
        }
    }

    property Gradient positive: Gradient {
        GradientStop {
            position: 0.0
            color: "#009100"
        }
        GradientStop {
            position: 0.15
            color: "#25009100"
        }
        GradientStop {
            position: 0.85
            color: "#25009100"
        }
        GradientStop {
            position: 0.0
            color: "#009100"
        }
    }

    property Gradient negative: Gradient {
        GradientStop {
            position: 0.0
            color: "#910000"
        }
        GradientStop {
            position: 0.15
            color: "#25910000"
        }
        GradientStop {
            position: 0.85
            color: "#25910000"
        }
        GradientStop {
            position: 0.0
            color: "#910000"
        }
    }

    property Gradient selected: Gradient {
        GradientStop {
            position: 0.0
            color: "#CDFCFF"
        }
        GradientStop {
            position: 0.5
            color: "#999999"
        }
        GradientStop {
            position: 1.0
            color: "#CDFCFF"
        }
    }

    property Gradient background: Gradient {
        GradientStop {
            position: 0.0
            color: "#D7F9CD"
        }
        GradientStop {
            position: 1.0
            color: "#FFFFFF"
        }
    }

    property Gradient header: Gradient {
        GradientStop {
            position: 0.0
            color: "#7A5C05"
        }
        GradientStop {
            position: 0.08
            color: "#96750C"
        }
        GradientStop {
            position: 0.25
            color: "#D4AF37"
        }
        GradientStop {
            position: 0.47
            color: "#F6C842"
        }
        GradientStop {
            position: 0.68
            color: "#FFD700"
        }
        GradientStop {
            position: 0.82
            color: "#FFECB3"
        }
        GradientStop {
            position: 0.92
            color: "#FFF8DC"
        }
        GradientStop {
            position: 1.0
            color: "#FFFDE8"
        }
    }


    property RadialGradient radioSelected: Gradient {
        GradientStop {
            position: 0.0
            color: "#F8F800"
        }
        GradientStop {
            position: 1.0
            color: "black"
        }
    }

    property Gradient black: Gradient {
        GradientStop {
            position: 0.0
            color: "black"
        }
        GradientStop {
            position: 1.0
            color: "black"
        }
    }
}
