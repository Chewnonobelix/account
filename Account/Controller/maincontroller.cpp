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

    QObject* root = m_engine.rootObjects().first();

    connect(root, SIGNAL(adding()), this, SLOT(add()));
    connect(root, SIGNAL(remove(int)), this, SLOT(remove(int)));
    connect(root, SIGNAL(edit(int)), this, SLOT(edit(int)));

    return 0;
}

void MainController::add()
{
    qDebug()<<"Add";
}

void MainController::remove(int id)
{
    qDebug()<<"Remove"<<id;
}

void MainController::edit(int id)
{
    qDebug()<<"Edit"<< id;
}
