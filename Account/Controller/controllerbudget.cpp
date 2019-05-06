#include "controllerbudget.h"

ControllerBudget::ControllerBudget()
{
    m_eng.load(QUrl(QStringLiteral("qrc:/BudgetManager.qml")));
    m_view = m_eng.rootObjects().first();
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

void ControllerBudget::openManager()
{
    if(m_view)
    {
        QMetaObject::invokeMethod(m_view, "clear");
        
        auto incomes = categories("income");
        auto outcomes = categories("outcome");
        
        qDebug()<<incomes<<outcomes;
        
        auto func = [&](QString type, QStringList list) {
            for(auto it: list)
            {
                QVariantMap map;
                map.insert("type", type);
                map.insert("catName", it);
//                map.insert("has", m_budgets.contains(it));
                map.insert("has", true);
                
                QMetaObject::invokeMethod(m_view, "add", Q_ARG(QVariant, map));
            }
        };
        
        func("income", incomes);
        func("outcome", outcomes);
        
        QMetaObject::invokeMethod(m_view, "show");
    }
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
    openManager();
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
