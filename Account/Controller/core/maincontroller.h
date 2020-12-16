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
#include "../ItemModel/categorylistmodel.h"
#include "../ItemModel/mainmodel.h"
#include "../data/controllerjson.h"
#include "../data/controllerxmlmulti.h"
#include "../data/xmltosql.h"
#include "../graph/abstractgraphcontroller.h"
#include "abstractcontroller.h"
#include "controllerinformation.h"
#include "controllersettings.h"
#include "controllertransfert.h"
#include "languagecontroller.h"
#include "liveqmlengine.h"
#include "../ItemModel/budgetmodel.h"
#include "../ItemModel/targetlistmodel.h"
#include "../ItemModel/budgetquickviewmodel.h"

class ACCOUNT_EXPORT MainController: public AbstractController
{
  Q_OBJECT

  Q_PROPERTY(int currentPage MEMBER m_currentPage)
		Q_PROPERTY(QList<QString> dateList READ dateList WRITE setDateList NOTIFY dateListChanged)

private:
  int m_currentPage = 1;
		QList<QString> m_dateList;

  LiveQmlEngine m_engine;
  ControllerInformation m_info;
  LanguageController m_lang;
  ControllerTransfert m_transfert;
  ControllerSettings m_settings;
  ControllerSynchronization m_synchro;
  QSharedPointer<CategoryListModel> m_categoryModel;
  QSharedPointer<MainModel> m_mainModel;
  AbstractGraphController m_graph;

  QList<QSharedPointer<FeatureBuilder>> m_features;

  void checkEstimated();


  QVariantList m_model;
  QMutex m_modelMutex;

  inline QQmlApplicationEngine &engine() { return m_engine.qmlEngine(); }
  inline QQmlApplicationEngine const &engine() const { return m_engine.qmlEngine(); }

public:
  MainController(int = 0);
  ~MainController();

		QList<QString> dateList() const;
		void setDateList(QList<QString>);

signals:
  void featuresListChanged(QStringList);
  void featuresChanged(QVariantList);
  void totaleChanged(QVariant);
  void profilesListChanged(QStringList);
  void currentProfileChanged(QString);
  void checkListChanged(QVariantList);
  void enableQuickView(bool);
  void accountChanged(QStringList);
  void currentAccountChanged(QString);
  void currentModelChanged(QVariantList);
  void maxPageChanged(int);
  void currentRowChanged(int);
  void clearCalendar();
  void appendCalendarPreview(QVariant);
  void appendMonthPreview(QVariant);
  void openAdd(bool);
		void dateListChanged();

public slots:
  int exec();

  void bind(QVariant);
  void add(bool = false);
  void remove(QVariant);
  void edit(QVariant);

  void adding(QVariant);

  void accountChange(QString);
  void addEntryMain(Entry);

  void update(Entry);

  void loadAccount();

  void validateCheckEstimated(QVariantList);
  void deleteAccount(QString);

  void previewCalendar(int = 0, int = 0);

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

  void sortRole(QString);
  void sortOrder(int);

  QString licence();
  QString readme();
  QString about();

  void reload();

  void totalChanged(int, int);
};


#endif // MAINCONTROLLER_H
