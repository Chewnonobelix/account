# Generated from Controller.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(Controller
    src/abstractcontroller.cpp
    src/abstractdatastorage.cpp
    src/controller.cpp
    src/inmemorystoragelayer.cpp
    src/jsonstoragelayer.cpp
    src/accountcontroller.cpp
    src/profilecontroller.cpp
    src/transactioncontroller.cpp
    src/categorycontroller.cpp
    src/debtcontroller.cpp
    src/frequencycontroller.cpp
    src/tricountcontroller.cpp
    src/budgetcontroller.cpp
    include/Controller/abstractcontroller.h
    include/Controller/abstractdatastorage.h
    include/Controller/abstractstoragelayer.h
    include/Controller/controller.h
    include/Controller/controller_global.h
    include/Controller/inmemorystoragelayer.h
    include/Controller/jsonstoragelayer.h
    include/Controller/storagecommand.h
    include/Controller/entitystorage.h
    include/Controller/accountcontroller.h
    include/Controller/profilecontroller.h
    include/Controller/transactioncontroller.h
    include/Controller/categorycontroller.h
    include/Controller/debtcontroller.h
    include/Controller/frequencycontroller.h
    include/Controller/tricountcontroller.h
    include/Controller/budgetcontroller.h
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
