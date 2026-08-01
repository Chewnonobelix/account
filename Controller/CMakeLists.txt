# Generated from Controller.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(Controller
    src/abstractcontroller.cpp
    src/abstractdatastorage.cpp
    src/controller.cpp
    src/inmemorystoragelayer.cpp
    include/Controller/abstractcontroller.h
    include/Controller/abstractdatastorage.h
    include/Controller/abstractstoragelayer.h
    include/Controller/controller.h
    include/Controller/controller_global.h
    include/Controller/inmemorystoragelayer.h
    include/Controller/storagecommand.h
)

target_include_directories(Controller PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(Controller PRIVATE
    CONTROLLER_LIBRARY
)

target_link_libraries(Controller PUBLIC
    Qt6::Core
    OAModel
)
