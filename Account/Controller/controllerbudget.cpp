#include "controllerbudget.h"

ControllerBudget::ControllerBudget()
{

}

void ControllerBudget::update(int id)
{
    QObject* view = m_views[id];

    if(view)
    {
        qDebug()<<"Update view budget";
        //TODO
    }
}

bool ControllerBudget::addTo(int id)
{
    bool ret = false;
    Entry e = entry(id);

    auto list = m_budgets.values(e.info().category());

    for(auto it: list)
        if(it.in(e.date()))
        {
            ret = true;
            it<<e;
            update(it.id());
        }

    return ret;
}

bool ControllerBudget::removeFrom(int id)
{
    bool ret = false;
    Entry e = entry(id);

    auto list = m_budgets.values(e.info().category());

    for(auto it: list)
        if(it.in(e.date()))
        {
            ret = true;
            it>>e;
            update(it.id());
        }

    return ret;
}

int ControllerBudget::exec()
{

    return 0;
}
