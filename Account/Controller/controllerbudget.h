#ifndef CONTROLLERBUDGET_H
#define CONTROLLERBUDGET_H

#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>

#include "Model/budget.h"
#include "abstractcontroller.h"

class ControllerBudget: public AbstractController
{
    Q_OBJECT

private:
    QQmlApplicationEngine m_eng;
    QObject* m_view, *m_referenceView, *m_quickView;
    QMap<QString, QObject*> m_views;
    QMap<QString, Budget> m_budgets;
    QDate m_currentDate;
    QString m_selected;
    
    void update(int);
    
public:
    ControllerBudget();
    ~ControllerBudget();
    bool addTo(int);
    bool removeFrom(int);

    int exec();
    void openManager();
    void reload();
    void setQuickView(QObject* );
    
public slots:
    void open(QString);
    void show(QDate);
    void closeManager();
    void addTarget(QString);
    void removeTarget(QString, QString);
    void editReference();
    void showTarget(QString, QString, bool);
//    void addBudget(QString, QDate, Account::FrequencyEnum);

    void addBudget(QString);
    void editBudget(QString);
    void getTarget(QString);

    void changeFrequency(QString, int);
    
    void updateEntry(int);
    void changeEntry(QString, int);
};

#endif // CONTROLLERBUDGET_H
