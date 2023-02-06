QT += testlib quick sql core xml widgets qml gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    main.cpp \
    testcategory.cpp \
    testdebt.cpp \
    testentry.cpp \
    testfrequency.cpp \
    testinformation.cpp

HEADERS +=  \
    testcategory.h \
    testdebt.h \
    testentry.h \
    testfrequency.h \
    testinformation.h

INCLUDEPATH = ../Account

RESOURCES += \
    ../Account/View/Account.qrc

DISTFILES += \


INCLUDEPATH += $$PWD/../Account
LIBS += -L$$OUT_PWD/../AccountApp -lLibAccount

message($$PWD)
message($$OUT_PWD)
INCLUDEPATH += $$PWD/../DesignLibrary/DesignPattern
DEPENDPATH += $$OUT_PWD/../DesignLibrary/DesignPattern/
LIBS += -L$$OUT_PWD/../DesignLibrary/DesignPattern/ -lDesignPattern
