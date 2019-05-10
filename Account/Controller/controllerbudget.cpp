#include "controllerbudget.h"

ControllerBudget::ControllerBudget()
{
    m_eng.load(QUrl(QStringLiteral("qrc:/Budget/BudgetManager.qml")));
    m_view = m_eng.rootObjects().first();

    connect(m_view, SIGNAL(s_budgetChanged(QString)), this, SLOT(addBudget(QString)));
    connect(m_view, SIGNAL(s_budgetReference(QString)), this, SLOT(editBudget(QString)));
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
                
        auto func = [&](QString type, QStringList list) {
            for(auto it: list)
            {
                QVariantMap map;
                map.insert("type", type);
                map.insert("catName", it);
                map.insert("has", m_budgets.contains(it));
//                map.insert("has", true);
                
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

//void ControllerBudget::addBudget(QString cat, QDate date, Account::FrequencyEnum f)
//{
//    if(!m_budgets.contains(cat))
//    {
//        Budget b;
//        b.setCategory(cat);
//        b.setReference(date);
//        b.setFrequency(f);
//        m_budgets[cat] = b;
//    }
//}

void ControllerBudget::addBudget(QString name)
{
    qDebug()<<name;

    if(m_budgets.contains(name))
    {
        m_db->removeBudget(m_budgets[name]);        
        m_budgets.remove(name);
    }
    else
    {
        Budget b;
        m_budgets[name] = b;
        m_db->addBudget(b);
    }

    openManager();
}

void ControllerBudget::editBudget(QString cat)
{
    qDebug()<<"Reference"<<cat;
    QQmlComponent referenceComp(&m_eng, QUrl(QStringLiteral("qrc:/Budget/ReferenceView.qml"))); 
    QObject* obj = referenceComp.create();
    qDebug()<<obj;
    QMetaObject::invokeMethod(obj, "show");
    
}
