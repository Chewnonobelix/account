#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQuickView>
#include <QVariant>
#include <QJSValue>
#include <QThread>
#include "abstractcontroller.h"
#include "controllerinformation.h"
#include "graphcontroller.h"
#include "controllerxmlmulti.h"
#include "xmltosql.h"
#include "languagecontroller.h"
#include "controllertransfert.h"
#include "controllerbudget.h"
#include "controllerfrequency.h"
#include "controllercommon.h"

class MainController: public AbstractController
{
    Q_OBJECT

private:
    QQmlApplicationEngine m_engine;
    ControllerInformation m_info;
    GraphController m_graph;
    LanguageController m_lang;
    ControllerTransfert m_transfert;
    ControllerBudget m_budget;
    ControllerFrequency m_freqs;
    ControllerCommon m_common;
    
    QThread m_dbThread;
    
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
    void addEntryMain(Entry);

    void toXml();

    void update(Entry);

    void loadAccount();

    void validateCheckEstimated();
    void deleteAccount(QString);

    void previewCalendar();

    void receiveSum();

    void openTransfert();

    void openBudgetManager();
    
    void close();

    void quickAdding();
    void quickAddCategory(QString);
    void quickOpen();

    void changeProfile(QString);
};


#endif // MAINCONTROLLER_H
