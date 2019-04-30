#ifndef CONTROLLERBUDGET_H
#define CONTROLLERBUDGET_H

#include <QQmlApplicationEngine>


#include "Model/budget.h"
#include "abstractcontroller.h"

class ControllerBudget: public AbstractController
{
    Q_OBJECT

private:
    QQmlApplicationEngine m_eng;
    QObject* m_view;
    QMap<int, QObject*> m_views;
    QMap<QString, Budget> m_budgets;
    QDate m_currentDate;

    void update(int);
public:
    ControllerBudget();
    ~ControllerBudget();
    bool addTo(int);
    bool removeFrom(int);

    int exec();

public slots:
    void open(QString);
    void show(QDate);
    void closeManager();
    void addTarget(QString, QDate, double);
    void removeTarget(QString, QDate);
    void addBudget(QString, QDate, Account::FrequencyEnum);
    void removeBudget(QString);
};

#endif // CONTROLLERBUDGET_H
