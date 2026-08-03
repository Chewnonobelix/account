# Generated from ViewTests.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(ViewTests
    tst_view.cpp
)

target_include_directories(ViewTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/DesignLibrary/DesignPattern
)

target_link_libraries(ViewTests PRIVATE
    Qt6::Test
    View
)

add_test(NAME ViewTests COMMAND ViewTests)

# Qt Quick Test: exercises the OB* QML components directly (unit +
# functional/interaction tests). Files matching qml/tst_*.qml are
# auto-discovered at run time from QUICK_TEST_SOURCE_DIR; they are also
# listed here so they show up in the IDE's project tree.
add_executable(UiKitComponentTests
    tst_uikit_components.cpp
    qml/tst_OBButton.qml
    qml/tst_OBCheckBox.qml
    qml/tst_OBComboBox.qml
    qml/tst_OBHeader.qml
    qml/tst_OBLabel.qml
    qml/tst_OBSpinBox.qml
    qml/tst_OBSwitch.qml
    qml/tst_OBTextInput.qml
    qml/tst_OBTitle.qml
    qml/tst_DescLine.qml
    qml/tst_TableRow.qml
)

target_compile_definitions(UiKitComponentTests PRIVATE
    QUICK_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/qml"
)

target_link_libraries(UiKitComponentTests PRIVATE
    Qt6::QuickTest
)

add_test(NAME UiKitComponentTests COMMAND UiKitComponentTests)
set_tests_properties(UiKitComponentTests PROPERTIES
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
)
