QT += testlib quick sql core xml widgets charts qml gui xmlpatterns

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    ../Account/Controller/abstractcontroller.cpp \
    ../Account/Controller/controllerdb.cpp \
    ../Account/Controller/controllerxmlmulti.cpp \
    ../Account/Controller/interfacedatasave.cpp \
    ../Account/Model/accountglobal.cpp \
    ../Account/Model/budget.cpp \
    ../Account/Model/categories.cpp \
    ../Account/Model/commonexpanse.cpp \
    ../Account/Model/entry.cpp \
    ../Account/Model/frequency.cpp \
    ../Account/Model/information.cpp \
    ../Account/Model/metadata.cpp \
    ../Account/Model/subbudget.cpp \
    ../Account/Model/total.cpp \
    main.cpp \
    testinformation.cpp

HEADERS +=  \
    ../Account/Controller/abstractcontroller.h \
    ../Account/Controller/controllerdb.h \
    ../Account/Controller/controllerxmlmulti.h \
    ../Account/Controller/interfacedatasave.h \
    ../Account/Model/accountglobal.h \
    ../Account/Model/budget.h \
    ../Account/Model/categories.h \
    ../Account/Model/commonexpanse.h \
    ../Account/Model/entry.h \
    ../Account/Model/frequency.h \
    ../Account/Model/information.h \
    ../Account/Model/metadata.h \
    ../Account/Model/subbudget.h \
    ../Account/Model/total.h \
    testinformation.h

INCLUDEPATH = ../Account

RESOURCES += \
    ../Account/View/Account.qrc

DISTFILES += \


