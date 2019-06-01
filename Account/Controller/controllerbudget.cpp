#include "controllerbudget.h"

ControllerBudget::ControllerBudget()
{
    m_eng.load(QUrl(QStringLiteral("qrc:/Budget/BudgetManager.qml")));
    m_view = m_eng.rootObjects().first();
    m_referenceView = nullptr;

    connect(m_view, SIGNAL(s_loadTarget(QString)), this, SLOT(getTarget(QString)));
    connect(m_view, SIGNAL(s_budgetChanged(QString)), this, SLOT(addBudget(QString)));
    connect(m_view, SIGNAL(s_budgetReference(QString)), this, SLOT(editBudget(QString)));
    connect(m_view, SIGNAL(s_budgetRoleChange(QString, int)), this, SLOT(changeFrequency(QString, int)));
    connect(m_view, SIGNAL(s_addTarget(QString)), this, SLOT(addTarget(QString)));
    connect(m_view, SIGNAL(s_showTarget(QString, QString, bool)), this, SLOT(showTarget(QString,QString,bool)));

}

ControllerBudget::~ControllerBudget()
{
}

void ControllerBudget::closeManager()
{    
    if(m_referenceView)
        QMetaObject::invokeMethod(m_referenceView, "close");
    
    if(m_view)
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
    Entry e = entry(id);

    bool ret = m_budgets.contains(e.info().category());

    if(m_budgets.contains(e.info().category()))
        m_budgets[e.info().category()]<<(e);

    return ret;
}

void ControllerBudget::openManager()
{
    if(m_view)
    {
        QMetaObject::invokeMethod(m_view, "clearCat");
        m_view->setProperty("blocked", true);
        auto incomes = categories("income");
        auto outcomes = categories("outcome");
                
        auto func = [&](QString type, QStringList list) {
            for(auto it: list)
            {
                QVariantMap map;
                map.insert("type", type);
                map.insert("catName", it);
                map.insert("has", m_budgets.contains(it));
                map.insert("frequency", m_budgets.contains(it) ? (int)m_budgets[it].frequency() : 0);
//                map.insert("has", true);
                
                QMetaObject::invokeMethod(m_view, "addCat", Q_ARG(QVariant, map));
            }
        };
        
        func("income", incomes);
        func("outcome", outcomes);
        
        QMetaObject::invokeMethod(m_view, "show");
        
        if(!m_selected.isEmpty())
            QMetaObject::invokeMethod(m_view, "selectCat", Q_ARG(QVariant, m_selected));

        m_view->setProperty("blocked", false);
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
    reload();
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

void ControllerBudget::reload()
{
    m_budgets.clear();;

    auto l = m_db->selectBudgets();

    for(auto b: l)
    {
        m_budgets[b.category()] = b;
    }

    auto e = m_db->selectEntry(currentAccount());
    for(auto it: e)
        addTo(it.id());
}

void ControllerBudget::addTarget(QString cat)
{
    editBudget(cat);
}

void ControllerBudget::removeTarget(QString cat, QDate date)
{
    if(m_budgets.contains(cat))
        m_budgets[cat].removeTarget(date);
}

void ControllerBudget::addBudget(QString name)
{
    if(m_budgets.contains(name))
    {
        m_db->removeBudget(m_budgets[name]);        
        m_budgets.remove(name);
    }
    else
    {
        Budget b;
        b.setCategory(name);
        m_budgets[name] = b;
        m_db->addBudget(b);
    }
    m_selected = name;
    
    reload();
    openManager();

}

void ControllerBudget::editBudget(QString cat)
{
    if(!m_referenceView)
    {
        QQmlComponent referenceComp(&m_eng, QUrl(QStringLiteral("qrc:/Budget/ReferenceView.qml")));
        m_referenceView = referenceComp.create();
        QObject* ok = m_referenceView->findChild<QObject*>("okButton");
        connect(ok, SIGNAL(clicked()), this , SLOT(editReference()));
    }
    m_referenceView->setProperty("budgetName", cat);

    QMetaObject::invokeMethod(m_referenceView, "show");
    
}

void ControllerBudget::editReference()
{
    QString cat = m_referenceView->property("budgetName").toString();
    QDate d; double val;
    QObject* obj = m_referenceView->findChild<QObject*>("cButton");
    d = QDate::fromString(obj->property("text").toString(), "dd-MM-yyyy");
    obj = m_referenceView->findChild<QObject*>("targetValue");
    val = obj->property("realValue").toDouble();
    QMetaObject::invokeMethod(m_referenceView, "close");
    m_budgets[cat].addTarget(d, val);
    m_db->updateBudget(m_budgets[cat]);
}

void ControllerBudget::getTarget(QString catName)
{
    auto list = m_budgets[catName].targets();


    QMetaObject::invokeMethod(m_view, "clearTarget");

    for(auto it = list.begin(); it != list.end(); it++)
    {
        QVariantMap map;
        map.insert("date", it.key());
        map.insert("target", it.value());

        QMetaObject::invokeMethod(m_view, "addTarget", Q_ARG(QVariant, map));
    }

    showTarget(catName, "", true);
}

void ControllerBudget::changeFrequency(QString cat, int freq)
{
    m_budgets[cat].setFrequency((Account::FrequencyEnum)freq);
    m_db->updateBudget(m_budgets[cat]);
}

void ControllerBudget::updateEntry(int id)
{
    Entry e = entry(id);
    
    if(m_budgets.contains(e.info().category()))
        m_budgets[e.info().category()]<<e;
}

void ControllerBudget::changeEntry(QString old, int id)
{
    Entry e = entry(id);
    
    if(m_budgets.contains(old))
        m_budgets[old]>>e;
    
    if(m_budgets.contains(e.info().category()))
        m_budgets[e.info().category()]<<e;
}

void ControllerBudget::showTarget(QString catName, QString date, bool all)
{
    QList<SubBudget> list2 ;
    QMetaObject::invokeMethod(m_view, "clearSub");

    auto list = m_budgets[catName].subs();
    if(!all)
    {
        QDate d = QDate::fromString(date, "dd-MM-yyyy");
        double target = m_budgets[catName].targets()[d];

        for(auto it: list)
            if(it.target() == target)
                list2<<it;
    }
    else
    {
        list2 = list.values();
    }

    for(auto it: list2)
    {
        QVariantMap map;
        map["begin"] = it.begin();
        map["end"] = it.end();
        map["current"] = it.current();
        map["target"] = it.target();
        map["cat"] = catName;
        QMetaObject::invokeMethod(m_view, "addSub", Q_ARG(QVariant, map));
    }
}
