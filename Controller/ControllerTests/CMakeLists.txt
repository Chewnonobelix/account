# Generated from ControllerTests.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(ControllerTests
    tst_controller.cpp
)

target_link_libraries(ControllerTests PRIVATE
    Qt6::Test
    Controller
)

add_test(NAME ControllerTests COMMAND ControllerTests)

add_executable(StorageLayerTests
    tst_storagelayer.cpp
)

target_link_libraries(StorageLayerTests PRIVATE
    Qt6::Test
    Controller
)

add_test(NAME StorageLayerTests COMMAND StorageLayerTests)

add_executable(JsonStorageLayerTests
    tst_jsonstoragelayer.cpp
)

target_link_libraries(JsonStorageLayerTests PRIVATE
    Qt6::Test
    Controller
)

add_test(NAME JsonStorageLayerTests COMMAND JsonStorageLayerTests)

add_executable(EntityStorageTests tst_entitystorage.cpp)
add_executable(AccountControllerTests tst_accountcontroller.cpp)
add_executable(ProfileControllerTests tst_profilecontroller.cpp)
add_executable(TransactionControllerTests tst_transactioncontroller.cpp)
add_executable(CategoryControllerTests tst_categorycontroller.cpp)
add_executable(DebtControllerTests tst_debtcontroller.cpp)
add_executable(FrequencyControllerTests tst_frequencycontroller.cpp)
add_executable(TricountControllerTests tst_tricountcontroller.cpp)
add_executable(BudgetControllerTests tst_budgetcontroller.cpp)

foreach(controllerTest
    EntityStorageTests
    AccountControllerTests
    ProfileControllerTests
    TransactionControllerTests
    CategoryControllerTests
    DebtControllerTests
    FrequencyControllerTests
    TricountControllerTests
    BudgetControllerTests
)
    target_link_libraries(${controllerTest} PRIVATE
        Qt6::Test
        Controller
    )
    add_test(NAME ${controllerTest} COMMAND ${controllerTest})
endforeach()
