import QtQuick

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

    }

    property Gradient positive: Gradient {

    }

    property Gradient negative: Gradient {

    }

    property Gradient selected: Gradient {

    }

    property Gradient background: Gradient {

    }

    property Gradient headerUp: Gradient {

    }

    property Gradient headerDown: Gradient {

    }

    property Gradient radioSelected: Gradient {

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
