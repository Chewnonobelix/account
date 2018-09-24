import QtQuick 2.0

QtObject {
    property Gradient goldButton: Gradient {
        id: goldButton
        GradientStop {
            color: "gold"
            position: 0.0
        }
        GradientStop {
            color: "goldenrod"
            position: 0.5
        }
        GradientStop {
            color: "gold"
            position: 1.0
        }

    }

    property Gradient goldHeader: Gradient {
        id: goldHeader
        GradientStop {
            color: "goldenrod"
            position: 0.0
        }
        GradientStop {
            color: "darkgoldenrod"
            position: 0.25
        }
        GradientStop {
            color: "gold"
            position: 0.5
        }
        GradientStop {
            color: "goldenrod"
            position: 0.75
        }
        GradientStop {
            color: "darkgoldenrod"
            position: 1.0
        }
    }

    property Gradient selectViewOut: Gradient {
        id:selectViewOut
        GradientStop {
            color: "gold"
            position: 0.0
        }
        GradientStop {
            color: "lightcoral"
            position: 0.5
        }
        GradientStop {
            color: "gold"
            position: 1.0        }
    }

    property Gradient selectViewIn: Gradient {
        id:selectViewIn
        GradientStop {
            color: "gold"
            position: 0.0
        }
        GradientStop {
            color: "aquamarine"
            position: 0.5
        }
        GradientStop {
            color: "gold"
            position: 1.0        }
    }

    property Gradient unselectView: Gradient {
        id: unselectView
        GradientStop {
            color: "transparent"
        }
    }

    property Gradient backgroundGradient:     Gradient {
        id: backgroundGradient
        GradientStop {
            color: "darkseagreen"
            position: 0.0
        }
        GradientStop {
            color: "white"
            position: 1
        }
    }

}
