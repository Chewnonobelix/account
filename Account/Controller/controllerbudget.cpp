#include "controllerbudget.h"

ControllerBudget::ControllerBudget()
{
    m_eng.load(QUrl(QStringLiteral("qrc:/BudgetManager.qml")));
    m_view = m_eng.rootObjects().first();
    QMetaObject::invokeMethod(m_view, "show");
}

ControllerBudget::~ControllerBudget()
{
}

void ControllerBudget::closeManager()
{
    QMetaObject::invokeMethod(m_view, "close");
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

    if(m_budgets.contains(e.info().category()))
        ret = m_budgets[e.info().category()].addEntry(e);

    return ret;
}

bool ControllerBudget::removeFrom(int id)
{
    bool ret = false;
    Entry e = entry(id);

    if(m_budgets.contains(e.info().category()))
        ret = m_budgets[e.info().category()].removeEntry(e);

    return ret;
}

int ControllerBudget::exec()
{
    //TODO
    return 0;
}

void ControllerBudget::open(QString cat)
{
    //TODO0
}

void ControllerBudget::show(QDate date)
{
    //TODO
}


void ControllerBudget::addTarget(QString cat, QDate date, double target)
{
    if(m_budgets.contains(cat))
        m_budgets[cat].addTarget(date, target);
}

void ControllerBudget::removeTarget(QString cat, QDate date)
{
    if(m_budgets.contains(cat))
        m_budgets[cat].removeTarget(date);
}

void ControllerBudget::addBudget(QString cat, QDate date, Account::FrequencyEnum f)
{
    if(!m_budgets.contains(cat))
    {
        Budget b;
        b.setCategory(cat);
        b.setReference(date);
        b.setFrequency(f);
        m_budgets[cat] = b;
    }
}

void ControllerBudget::removeBudget(QString cat)
{
    if(m_budgets.contains(cat))
        m_budgets.remove(cat);
}
