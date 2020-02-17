#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQuickView>
#include <QVariant>
#include <QJSValue>
#include <QThread>
#include <QQmlComponent>

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
#include "controllersettings.h"

class MainController: public AbstractController
{
    Q_OBJECT

private:
    QQmlApplicationEngine m_engine;
    ControllerInformation m_info;
    GraphController m_graph;
    LanguageController m_lang;
    ControllerTransfert m_transfert;
    ControllerSettings m_settings;

    QList<QSharedPointer<FeatureBuilder>> m_features;
    
    QThread m_dbThread;
    
    void checkEstimated();

    QList<QDate> dateList() const;

    QVariantList m_model;

public:
    MainController(int = 0);
    ~MainController();


public slots:
    int exec();

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
    
    void close();

    void quickAdding();
    void quickAddCategory(QString);
    void quickOpen();

    void changeProfile(QString);
    void addProfile();
    void loadProfiles();
    void deleteProfile(QString);
    
    void languageChange();
    
    void loadFeatures();
    
    void updateQuickView();

    void buildModel(int = -1);
    void pageChange(int = -1);
};


#endif // MAINCONTROLLER_H
