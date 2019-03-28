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

void ControllerBudget::open(QString cat)
{

}

void ControllerBudget::show(QDate date)
{
    QStringList catList = m_budgets.keys();
    QList<Budget> currentBudget;

    for(auto it: catList)
        for(auto it2: m_budgets.values(it))
            if(it2.in(date))
                currentBudget<<it2;



    for(auto i = 0; i < currentBudget.size(); i ++)
    {
        QVariantMap map;
        map.insert("target", currentBudget[i].target());
        map.insert("currentValue", currentBudget[i].current());
        map.insert("name", currentBudget[i].category());
        map.insert("date", currentBudget[i].start());
        QMetaObject::invokeMethod(m_view, "add", Q_ARG(QVariant, map), Q_ARG(QVariant, i%2));
    }
}
