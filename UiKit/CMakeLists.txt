set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(UiKit
    src/main.cpp
)

# UiKit is a dev-only playground: LiveQmlEngine (ChewnoDesignPattern) hot-reloads
# QML straight from these source directories instead of a compiled qrc.
target_compile_definitions(UiKit PRIVATE
    UIKIT_QML_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/QML"
    VIEW_QML_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../View/QML"
)

target_link_libraries(UiKit PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    ChewnoDesignPattern
)
