#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQuickView>
#include <QVariant>
#include <QJSValue>
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

public slots:
    void add();
    void remove(int);
    void edit(int);

    void adding();
    void selection();
};

#endif // MAINCONTROLLER_H
