#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QRandomGenerator>
#include "Controller/maincontroller.h"
//#include "Test/xmltest.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    qDebug()<<"ControllerDB"<<qRegisterMetaType<ControllerDB>()<<sizeof(int*);
    qDebug()<<"ControllerXML"<<qRegisterMetaType<ControllerXML>();

    /*QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    qDebug()<<engine.rootObjects();*/
    MainController mc;

    mc.exec();

//    XmlTest test1;

    if(argc > 1 && QString(argv[1]) == "--test")
    {
//        test1.start();
    }

    return app.exec();
}
