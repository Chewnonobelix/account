cmake_minimum_required(VERSION 3.16)
project(OpenAccount LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OPENACCOUNT_QT_COMPONENTS
    Core
    Gui
    Test
    Qml
    Quick
    QuickControls2
    Network
    Sql)

find_package(Qt6 QUIET CONFIG COMPONENTS ${OPENACCOUNT_QT_COMPONENTS})

if(NOT Qt6_FOUND)
    set(_openaccount_qt_search_roots
        "$ENV{Qt6_DIR}"
        "$ENV{QTDIR}"
        "$ENV{QT_ROOT}"
        "$ENV{QT_DIR}"
        "$ENV{HOME}/Qt"
        "/opt/Qt"
        "/opt/qt")
    set(_openaccount_qt_suffixes
        "lib/cmake/Qt6"
        "lib64/cmake/Qt6"
        "gcc_64/lib/cmake/Qt6")

    unset(_openaccount_qt6_dir)
    foreach(_openaccount_qt_root IN LISTS _openaccount_qt_search_roots)
        if(NOT _openaccount_qt_root OR NOT EXISTS "${_openaccount_qt_root}")
            continue()
        endif()

        if(EXISTS "${_openaccount_qt_root}/Qt6Config.cmake")
            set(_openaccount_qt6_dir "${_openaccount_qt_root}")
        else()
            find_path(_openaccount_qt6_dir
                NAMES Qt6Config.cmake
                PATHS "${_openaccount_qt_root}"
                PATH_SUFFIXES ${_openaccount_qt_suffixes}
                NO_DEFAULT_PATH)
        endif()

        if(NOT _openaccount_qt6_dir)
            file(GLOB _openaccount_qt_children LIST_DIRECTORIES true "${_openaccount_qt_root}/*")
            foreach(_openaccount_qt_child IN LISTS _openaccount_qt_children)
                if(NOT IS_DIRECTORY "${_openaccount_qt_child}")
                    continue()
                endif()

                find_path(_openaccount_qt6_dir
                    NAMES Qt6Config.cmake
                    PATHS "${_openaccount_qt_child}"
                    PATH_SUFFIXES ${_openaccount_qt_suffixes}
                    NO_DEFAULT_PATH)
                if(_openaccount_qt6_dir)
                    break()
                endif()
            endforeach()
        endif()

        if(_openaccount_qt6_dir)
            get_filename_component(_openaccount_qt_prefix "${_openaccount_qt6_dir}/../../.." ABSOLUTE)
            set(Qt6_DIR "${_openaccount_qt6_dir}" CACHE PATH "Directory containing Qt6Config.cmake" FORCE)
            list(PREPEND CMAKE_PREFIX_PATH "${_openaccount_qt_prefix}")
            list(REMOVE_DUPLICATES CMAKE_PREFIX_PATH)
            message(STATUS "Using Qt6 from ${_openaccount_qt_prefix}")
            break()
        endif()
    endforeach()
endif()

find_package(Qt6 REQUIRED CONFIG COMPONENTS ${OPENACCOUNT_QT_COMPONENTS})
# Note: components above are a superset; you can trim later.
# include(CTest)

add_subdirectory(lib/DesignLibrary)
add_subdirectory(Controller)
add_subdirectory(Model)
# add_subdirectory(View)
add_subdirectory(app/OpenAccount)
add_subdirectory(Controller/ControllerTests)
add_subdirectory(Model/ModelTests)
# add_subdirectory(View/ViewTests)
