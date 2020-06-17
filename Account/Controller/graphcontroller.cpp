#include "graphcontroller.h"

TimeGraphController::TimeGraphController(): AbstractController(), m_view(nullptr), m_gran(Account::Month)
{

}

TimeGraphController::~TimeGraphController()
{
}

int TimeGraphController::exec()
{
    return 0;
}


void TimeGraphController::setDate(QDate d)
{
    m_view->setProperty("currentDate", d);
}

void TimeGraphController::setGran(Account::Granularity g)
{
    m_view->setProperty("currentGran", g);
    m_gran = g;

    if(m_gran == Account::Year) m_view->setProperty("granularity", tr("one year"));
    if(m_gran == Account::Month) m_view->setProperty("granularity", tr("one month"));
    if(m_gran == Account::Over) m_view->setProperty("granularity", tr("all years"));
}

void TimeGraphController::add(const Entry & e)
{
    QMetaObject::invokeMethod(m_view, "clear");

    if(m_sum.isEmpty())
    {
        auto ent = m_db->selectEntry(currentAccount());
        Total t;
        t.setDate(ent.begin().key().addDays(-1));

        for(auto it = ent.begin(); it != ent.end() && it->date() < e.date(); it++)
            t = t + *it;
        m_sum[t.date()] = t;
    }
    
    QDate temp;
    switch(m_gran)
    {
    case Account::Month:
        if(!m_sum.contains(e.date()))
            m_sum[e.date()] = m_sum[e.date().addDays(-1)];

        m_sum[e.date()] = m_sum[e.date()] +  e;
        break;

    case Account::Over:
    case Account::Year:
        temp = e.date();
        temp = temp.addDays(-(e.date().day()-1));
        
        if(!m_sum.contains(temp) && m_sum.count() == 1)
            m_sum[temp] = m_sum[temp.addDays(-1)];
        else if(!m_sum.contains(temp))
            m_sum[temp] = m_sum[temp.addMonths(-1)];            
        
        m_sum[temp] = m_sum[temp] + e;
        break;
    }
}

void TimeGraphController::update()
{
    if(m_sum.isEmpty())
        return;
    
    QDate mind = (m_sum.begin()+1).key(), maxd = m_sum.lastKey();

    double min = m_sum.first().value(), max = m_sum.first().value();
    for(auto it = m_sum.begin()+1; it != m_sum.end(); it++)
    {
        min = it->value() < min ? it->value() : min;
        max = it->value() > max ? it->value() : max;

        if(it.key() <= QDate::currentDate())
            QMetaObject::invokeMethod(m_view, "addDataMain", Q_ARG(QVariant,it.key()), Q_ARG(QVariant, it.value().value()));

        if((it.key() > QDate::currentDate()) || (it.key() <= QDate::currentDate() && (it+1).key() > QDate::currentDate()))
            QMetaObject::invokeMethod(m_view, "addDataEstimated", Q_ARG(QVariant,it.key()), Q_ARG(QVariant, it.value().value()));

    }

    QMetaObject::invokeMethod(m_view, "setMinMaxDate", Q_ARG(QVariant, mind), Q_ARG(QVariant, maxd));
    QMetaObject::invokeMethod(m_view, "setMinMaxValue", Q_ARG(QVariant, min), Q_ARG(QVariant, max));
}

void TimeGraphController::clear()
{
    m_sum.clear();
}

void TimeGraphController::setView(const QQmlApplicationEngine & engine)
{
    m_view = engine.rootObjects().first()->findChild<QObject*>("timeGraph");
}
