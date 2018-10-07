#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQuickView>
#include <QVariant>
#include <QJSValue>
#include "abstractcontroller.h"
#include "controllerinformation.h"

class MainController: AbstractController
{
    Q_OBJECT

private:
    QQmlApplicationEngine m_engine;
    ControllerInformation m_info;

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
    void accountChange(QString);

    void toXml(bool);
};

#endif // MAINCONTROLLER_H
