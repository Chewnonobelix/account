import QtQuick 2.0

QtObject {
    property FontLoader title: FontLoader{
        source: "CASTELAR.TTF"
//        name: "Segoe Script"
        id: titleFont
        property int size: 20
        property int size2: 16

    }

    property FontLoader core: FontLoader{
//        name: "Castellar"
        source: "BASKVILL.TTF"
        id: coreFont
        property int size: 15

    }

    property Gradient darkGoldButton: Gradient {
        id: darkGoldButton
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

    property Gradient goldButton: Gradient {
        id: goldButton
        GradientStop {
            color: "goldenrod"
            position: 0.0
        }
        GradientStop {
            color: "gold"
            position: 0.5
        }
        GradientStop {
            color: "goldenrod"
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

    property Gradient calSelect: Gradient {
        id: gradientSelect

        GradientStop {
            color: "darkseagreen"
            position: 0.0
        }

        GradientStop {
            color: "seagreen"
            position: 0.5
        }

        GradientStop {
            color: "darkseagreen"
            position: 1.0
        }
    }

}
