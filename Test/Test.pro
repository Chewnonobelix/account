QT += testlib quick sql core xml widgets charts qml gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_xmltest.cpp \
    ../Account/Controller/abstractcontroller.cpp \
    ../Account/Controller/controllerdb.cpp \
    ../Account/Controller/controllerinformation.cpp \
    ../Account/Controller/controllertransfert.cpp \
    ../Account/Controller/controllerxml.cpp \
    ../Account/Controller/controllerxmlmulti.cpp \
    ../Account/Controller/graphcontroller.cpp \
    ../Account/Controller/maincontroller.cpp \
    ../Account/Controller/languagecontroller.cpp \
    ../Account/Controller/xmltosql.cpp \
    ../Account/Model/budget.cpp \
    ../Account/Model/categories.cpp \
    ../Account/Model/commonexpense.cpp \
    ../Account/Model/entry.cpp \
    ../Account/Model/information.cpp \
    ../Account/Model/total.cpp


HEADERS +=  \
    ../Account/Controller/abstractcontroller.h \
    ../Account/Controller/controllerdb.h \
    ../Account/Controller/controllerinformation.h \
    ../Account/Controller/controllertransfert.h \
    ../Account/Controller/controllerxml.h \
    ../Account/Controller/controllerxmlmulti.h \
    ../Account/Controller/graphcontroller.h \
    ../Account/Controller/languagecontroller.h \
    ../Account/Controller/interfacedatasave.h \
    ../Account/Controller/maincontroller.h \
    ../Account/Controller/xmltosql.h \
    ../Account/Model/budget.h \
    ../Account/Model/categories.h \
    ../Account/Model/commonexpense.h \
    ../Account/Model/entry.h \
    ../Account/Model/information.h \
    ../Account/Model/total.h

INCLUDEPATH = ../Account

RESOURCES += \
    ../Account/View/qml.qrc

DISTFILES += \
    ../Account/View/BASKVILL.TTF \
    ../Account/View/CASTELAR.TTF \
    ../Account/View/qtquickcontrols2.conf \
    ../Account/View/AccountStyle.qml \
    ../Account/View/Adding.qml \
    ../Account/View/CheckEstimated.qml \
    ../Account/View/DoubleSpinBox.qml \
    ../Account/View/InformationView.qml \
    ../Account/View/main.qml \
    ../Account/View/MultiCalendar.qml \
    ../Account/View/Page1Form.qml \
    ../Account/View/Page2Form.qml \
    ../Account/View/Transfert.qml


