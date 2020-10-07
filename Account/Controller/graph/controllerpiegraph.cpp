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
    m_entries[Account::Income][""] = Total();
    m_entries[Account::Outcome][""] = Total();
}

void ControllerPieGraph::add(const Entry & e)
{
    //    Total t = m_entries[e.type()][e.category()];
    //    t = t + e;
    //    m_entries[e.type()][e.category()] = t;
}

void ControllerPieGraph::setGran(Account::Granularity) {}

void ControllerPieGraph::update()
{
    emit clearView();
    auto setter = [this](QMap<QString, Total> s, QString pie) {
        for (auto it = s.begin(); it != s.end(); it++) {
            emit append(pie, it.key().isEmpty() ? tr("Unknow") : it.key(), it.value().value());
        }
    };

    setter(m_entries[Account::Income], "income");
    setter(m_entries[Account::Outcome], "outcome");
}

void ControllerPieGraph::setView(const QQmlApplicationEngine & eng)
{
    auto *context = eng.rootContext();
    context->setContextProperty("_pie", this);
}
