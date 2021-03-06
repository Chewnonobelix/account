#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQuickView>
#include <QVariant>
#include <QJSValue>
#include "abstractcontroller.h"
#include "controllerinformation.h"
#include "controllerxml.h"
#include "graphcontroller.h"
#include "controllerxmlmulti.h"
#include "xmltosql.h"
#include "languagecontroller.h"

class MainController: public AbstractController
{
    Q_OBJECT

private:
    QQmlApplicationEngine m_engine;
    ControllerInformation m_info;
    GraphController m_graph;
    LanguageController m_lang;

    void checkEstimated();

public:
    MainController();
    ~MainController();

    int exec();

public slots:
    void add(bool = false);
    void remove(int);
    void edit(int);

    void adding();
    void selection(int = -1);
    void accountChange(QString);

    void toXml();

    void update(Entry);

    void addCategory(QString, QString);

    void loadAccount();

    void validateCheckEstimated();
    void deleteAccount(QString);

    void previewCalendar();

    void receiveSum();
};


#endif // MAINCONTROLLER_H
