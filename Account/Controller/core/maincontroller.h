#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQuickView>
#include <QVariant>
#include <QJSValue>
#include <QThread>
#include <QQmlComponent>

#include "../../account_global.h"
#include "../data/controllerxmlmulti.h"
#include "../data/xmltosql.h"
#include "../graph/abstractgraphcontroller.h"
#include "abstractcontroller.h"
#include "controllerinformation.h"
#include "controllersettings.h"
#include "controllertransfert.h"
#include "languagecontroller.h"
#include "liveqmlengine.h"
class Builder: public QThread
{
    Q_OBJECT
    
protected:
    void run() override;
    
public:
    Builder() = default;
    ~Builder() = default;
    QList<Entry> init;
    QVariantList* model;
    Total t;
    QMutex* m_mutex;
 };

class ACCOUNT_EXPORT MainController: public AbstractController
{
    Q_OBJECT

    
private:
    LiveQmlEngine m_engine;
    ControllerInformation m_info;
    LanguageController m_lang;
    ControllerTransfert m_transfert;
    ControllerSettings m_settings;
    ControllerSynchronization m_synchro;

    AbstractGraphController m_graph;
    
    QList<QSharedPointer<FeatureBuilder>> m_features;
        
    void checkEstimated();

    QList<QDate> dateList() const;

    QVariantList m_model;
    QSharedPointer<Builder> m_modelBuilder = nullptr;
    QMutex m_modelMutex;

    inline QQmlApplicationEngine &engine() { return m_engine.qmlEngine(); }
    inline QQmlApplicationEngine const &engine() const { return m_engine.qmlEngine(); }

public:
    MainController(int = 0);
    ~MainController();

signals:
    void featuresListChanged(QStringList);
    void featuresChanged(QVariantList);
    void totaleChanged(QVariant);
    void profilesListChanged(QStringList);
    void currentProfileChanged(QString);
    void checkListChanged(QVariantList);

public slots:
    int exec();

    void bind(QVariant);
    void add(bool = false);
    void remove(QVariant);
    void edit(QVariant);

    void adding();

    void accountChange(QString);
    void addEntryMain(Entry);

    void update(Entry);

    void loadAccount();

    void validateCheckEstimated(QVariantList);
    void deleteAccount(QString);

    void previewCalendar();

    void receiveSum();

    void openTransfert();
    
    void close();

    void quickAdding();
    void quickAddCategory(QString);
    void quickOpen();

    void changeProfile(QString);
    void addProfile(QString);
    void loadProfiles();
    void deleteProfile(QString);
    
    void languageChange();
    
    void loadFeatures();
    
    void updateQuickView();

    void buildModel(QUuid = QUuid());
    void pageChange(QUuid = QUuid());

    void sortRole(QString);
    void sortOrder(int);

    void licence();
    void readme();
    void about();

    void reload();

    void totalChanged();
};


#endif // MAINCONTROLLER_H
