#ifndef CONTROLLERBUDGET_H
#define CONTROLLERBUDGET_H


#include "Model/budget.h"
#include "abstractcontroller.h"

class ControllerBudget: public AbstractController
{
    Q_OBJECT

private:
    QObject* m_view;
    QMap<int, QObject*> m_views;
    QMultiMap<QString, Budget> m_budgets;
    QDate m_currentDate;

    void update(int);
public:
    ControllerBudget();
    bool addTo(int);
    bool removeFrom(int);

    int exec();

public slots:
    void open(QString);
    void show(QDate);
};

#endif // CONTROLLERBUDGET_H
