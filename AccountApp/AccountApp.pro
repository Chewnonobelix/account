CONFIG += c++17 
QT += quick sql core xml widgets charts qml gui xmlpatterns

TEMPLATE = app
TARGET = Account
SOURCES += \
    main.cpp

INCLUDEPATH += $$PWD/../Account

LIBS += -L$$OUT_PWD/ -lLibAccount


INCLUDEPATH += $$PWD/../DesignLibrary/DesignPattern
DEPENDPATH += $$OUT_PWD/../DesignLibrary/DesignPattern/debug
LIBS += -L$$OUT_PWD/../DesignLibrary/DesignPattern/debug -lDesignPattern

zip.path = $$OUT_PWD
zip.files = $$PWD/../7z.dll $$PWD/../7z.exe

INSTALLS += zip
