#include "Controller/core/maincontroller.h"
#include <QDebug>
#include <QGuiApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    QGuiApplication::setQuitOnLastWindowClosed(false);
    app.setApplicationName("Account");
    app.setOrganizationName("Chewnonobelix Inc");

    int index = app.arguments().indexOf("--s");
    int type = index > -1 ? app.arguments()[index + 1].toInt() : 0;
    MainController mc(type);

    mc.exec();

    return app.exec();
}
