#ifndef CONTROLLERBUDGET_H
#define CONTROLLERBUDGET_H


#include "Model/budget.h"
#include "abstractcontroller.h"

class ControllerBudget: public AbstractController
{
private:
    QMap<int, QObject*> m_views;
    QMultiMap<QString, Budget> m_budgets;

    void update(int);
public:
    ControllerBudget();
    bool addTo(int);
    bool removeFrom(int);

    int exec();
};

#endif // CONTROLLERBUDGET_H
