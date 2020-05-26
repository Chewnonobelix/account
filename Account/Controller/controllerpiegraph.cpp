#include "controllerpiegraph.h"

ControllerPieGraph::ControllerPieGraph(QObject*)
{
    
}

int ControllerPieGraph::exec()
{
    auto entries = m_db->selectEntry(currentAccount());
    
    int month = QDate::currentDate().month(), year = QDate::currentDate().year();
    
    QMap<QString, QMap<QString, Total>> sum;
    for(auto it: entries)
    {
        if(it.date().month() == month && it.date().year() == year)
        {
            Total t = sum[it.type()][it.info().category()];
            t = t + it;
            sum[it.type()][it.info().category()] = t;
        }
    }
    
    QMetaObject::invokeMethod(m_income, "clear"); 
    QMetaObject::invokeMethod(m_outcome, "clear");
    
    auto setter = [](QMap<QString, Total> s, QObject* pie) {
        for(auto it = s.begin(); it != s.end(); it++) 
        {
            QMetaObject::invokeMethod(pie, "append", Q_ARG(QVariant, it.key().isEmpty() ? tr("Unknow") : it.key()), Q_ARG(QVariant, it.value().value()));
        }
    };
    
    setter(sum["income"], m_income);
    setter(sum["outcome"], m_outcome);
}

void ControllerPieGraph::init(const QQmlApplicationEngine &engine)
{
    connect(m_db, InterfaceDataSave::s_updateEntry, this, ControllerPieGraph::exec);
    m_income = engine.rootObjects().first()->findChild<QObject*>("incomingPie");
    m_outcome = engine.rootObjects().first()->findChild<QObject*>("outcomePie");
}
