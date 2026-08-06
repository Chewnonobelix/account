# Generated from Model.pro

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(OAModel
    include/Model/enums.h
    include/Model/metatypes.h
    include/Model/model.h
    include/Model/model_global.h
    src/enums.cpp
    src/model.cpp

    # Business: domain entities (no Qt item-model interface).
    include/Model/Business/account.h
    include/Model/Business/budget.h
    include/Model/Business/category.h
    include/Model/Business/debt.h
    include/Model/Business/frequency.h
    include/Model/Business/profile.h
    include/Model/Business/total.h
    include/Model/Business/transaction.h
    include/Model/Business/transfer.h
    include/Model/Business/tricount.h
    include/Model/Business/tricountexpense.h
    include/Model/Business/tricountmember.h
    src/Business/account.cpp
    src/Business/budget.cpp
    src/Business/category.cpp
    src/Business/debt.cpp
    src/Business/frequency.cpp
    src/Business/profile.cpp
    src/Business/total.cpp
    src/Business/transaction.cpp
    src/Business/transfer.cpp
    src/Business/tricount.cpp
    src/Business/tricountexpense.cpp
    src/Business/tricountmember.cpp

    # Models: QAbstractItemModel-derived types, meant to be bound as QML
    # views/OBComboBox models.
    include/Model/Models/accountbalanceproxymodel.h
    include/Model/Models/accounttransactionfilterproxymodel.h
    include/Model/Models/transactionlistmodel.h
    src/Models/accountbalanceproxymodel.cpp
    src/Models/accounttransactionfilterproxymodel.cpp
    src/Models/transactionlistmodel.cpp
)

target_include_directories(OAModel PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    # The generated QML type registration code (oamodel_qmltyperegistrations.cpp)
    # includes headers by bare name (<enums.h>, <transaction.h>, ...), so
    # every directory that holds a QML_ELEMENT-tagged header must be on the
    # include path too. It also lets headers in one subfolder quote-include
    # a sibling in the other (e.g. transactionlistmodel.h -> transaction.h).
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Model
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Model/Business
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Model/Models
)

target_compile_definitions(OAModel PRIVATE
    "MODEL_LIBRARY"
)

target_link_libraries(OAModel PUBLIC
    Qt6::Core
    Qt6::Qml
    ChewnoDesignPattern
)

# Exposes OpenAccountEnums and Transaction to QML (import OpenAccount.Model)
# via the QML_NAMED_ELEMENT/QML_ELEMENT macros in
# enums.h/Business/transaction.h.
qt_add_qml_module(OAModel
    URI OpenAccount.Model
    VERSION 1.0
)
