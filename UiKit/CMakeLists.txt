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
    $<LINK_LIBRARY:WHOLE_ARCHIVE,OAModel>
)

# OAModel is a static QML module (import OpenAccount.Model). Its type
# registration lives in a translation unit nothing else references, so a
# normal static-archive link would drop it; qt6_import_qml_plugins() can't
# help here either since UiKit loads .qml files from disk at runtime rather
# than through qt_add_qml_module/QML_FILES, so qmlimportscanner finds
# nothing to scan. WHOLE_ARCHIVE forces that object file (and its static
# QQmlModuleRegistration) to be linked in unconditionally.
