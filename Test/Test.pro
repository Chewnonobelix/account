QT += testlib quick sql core xml widgets charts qml gui xmlpatterns

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
LIBS += -L$$OUT_PWD/ -lLibAccount

INCLUDEPATH += $$PWD/../DesignLibrary/DesignPattern
DEPENDPATH += $$OUT_PWD/../DesignLibrary/DesignPattern/debug
LIBS += -L$$OUT_PWD/../DesignLibrary/DesignPattern/debug -lDesignPattern
