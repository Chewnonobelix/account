#include "maincontroller.h"

MainController::MainController(): AbstractController()
{

}

MainController::~MainController()
{}

int MainController::exec()
{
    m_engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (m_engine.rootObjects().isEmpty())
        return -1;

    return 0;
}
