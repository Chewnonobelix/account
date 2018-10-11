#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QRandomGenerator>
#include "Controller/controllerdb.h"
#include "Controller/maincontroller.h"

void test()
{
//    ControllerDB cdbb;
//    qDebug()<<"Test";
//    auto accounts = cdbb.selectAccount();
//    qDebug()<<accounts;
//    auto entries1 = cdbb.selectEntry(accounts[0]);
//    qDebug()<<entries1.size()<<accounts[0];
//    for(auto e: entries1)
//        qDebug()<<e.id()<<e.value()<<e.date()<<e.type();

//    Entry e;
//    QRandomGenerator rdn;
//    rdn.bounded(999.9);
//    e.setAccount(accounts[0]);
//    e.setValue(rdn.generateDouble());
//    e.setDate(QDate::currentDate());
//    e.setType("outcome");
//    cdbb.addEntry(e);

//    auto entries2 = cdbb.selectEntry(accounts[0]);
//    qDebug()<<entries2.size()<<accounts[0];
//    for(auto e: entries2)
//        qDebug()<<e.id()<<e.value()<<e.date()<<e.type();



}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qDebug()<<"ControllerDB"<<qRegisterMetaType<ControllerDB>()<<sizeof(int*);
    qDebug()<<"ControllerXML"<<qRegisterMetaType<ControllerXML>();

    /*QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    qDebug()<<engine.rootObjects();*/
    MainController mc;

    mc.exec();

    if(argc > 1 && QString(argv[1]) == "--test")
        test();


    return app.exec();
}
