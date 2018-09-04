#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QQmlApplicationEngine>

#include "abstractcontroller.h"

class MainController: AbstractController
{
    Q_OBJECT

private:
    QQmlApplicationEngine m_engine;

public:
    MainController();
    ~MainController();

    int exec();
};

#endif // MAINCONTROLLER_H
