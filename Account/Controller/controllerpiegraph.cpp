#include "controllerpiegraph.h"

ControllerPieGraph::ControllerPieGraph(QObject*)
{

}

int ControllerPieGraph::exec()
{
    return 0;
}

void ControllerPieGraph::clear()
{
    m_entries.clear();
}

void ControllerPieGraph::setDate(QDate d)
{
    m_view->setProperty("currentDate", d);
}

void ControllerPieGraph::add(const Entry & e)
{
    Total t = m_entries[e.type()][e.info().category()];
    t = t + e;
    m_entries[e.type()][e.info().category()] = t;

    QMetaObject::invokeMethod(m_income, "clear");
    QMetaObject::invokeMethod(m_outcome, "clear");

    auto setter = [](QMap<QString, Total> s, QObject* pie) {
        for(auto it = s.begin(); it != s.end(); it++)
        {
            QMetaObject::invokeMethod(pie, "append", Q_ARG(QVariant, it.key().isEmpty() ? tr("Unknow") : it.key()), Q_ARG(QVariant, it.value().value()));
        }
    };

    setter(m_entries["income"], m_income);
    setter(m_entries["outcome"], m_outcome);
}

void ControllerPieGraph::setGran(Account::Granularity g)
{
    m_view->setProperty("gran", g);
}

void ControllerPieGraph::setView(const QQmlApplicationEngine & eng)
{
    m_income = eng.rootObjects().first()->findChild<QObject*>("incomingPie");
    m_outcome = eng.rootObjects().first()->findChild<QObject*>("outcomePie");
    m_view = eng.rootObjects().first()->findChild<QObject*>("pieCategory");
}


